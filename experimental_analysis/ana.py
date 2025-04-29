import os
import re
import argparse
import csv
from collections import defaultdict

def count_lines(filepath):
    """
    Return the number of lines in the given file. If file can't be opened, returns 0.
    """
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            return sum(1 for _ in f)
    except Exception:
        return 0


def extract_cwe(filename):
    """
    Extract CWE identifier from filename (e.g., CWE-89 or CWE89).
    Returns 'Unknown' if not found.
    """
    match = re.search(r"CWE[-_ ]?(\d+)", filename, re.IGNORECASE)
    return f"CWE{match.group(1)}" if match else "Unknown"


def analyze_pairwise(src_dir, result_dir):
    """
    Read .txt files from both directories in sorted order, pairwise.
    Returns per-file stats, average compression ratios, and empty file rates by CWE.
    """
    src_files = sorted([f for f in os.listdir(src_dir) if f.lower().endswith('.txt')])
    res_files = sorted([f for f in os.listdir(result_dir) if f.lower().endswith('.txt')])

    per_file_results = []
    ratios_by_cwe = defaultdict(list)
    empty_counts = defaultdict(int)
    total_counts = defaultdict(int)

    for src_fname, res_fname in zip(src_files, res_files):
        src_path = os.path.join(src_dir, src_fname)
        res_path = os.path.join(result_dir, res_fname)

        src_lines = count_lines(src_path)
        res_lines = count_lines(res_path)

        cwe = extract_cwe(src_fname)
        total_counts[cwe] += 1

        if res_lines > 0 and src_lines > 0:
            ratio = res_lines / src_lines
            ratios_by_cwe[cwe].append(ratio)
        else:
            ratio = None
            if res_lines == 0:
                empty_counts[cwe] += 1

        per_file_results.append({
            'SourceFile': src_fname,
            'ResultFile': res_fname,
            'CWE': cwe,
            'SourceLines': src_lines,
            'ResultLines': res_lines,
            'CompressionRatio': round(ratio, 4) if ratio is not None else None,
            'IsEmptyResult': res_lines == 0
        })

    avg_ratios = {cwe: (sum(rats) / len(rats)) if rats else None for cwe, rats in ratios_by_cwe.items()}
    empty_rates = {cwe: (empty_counts[cwe] / total_counts[cwe] if total_counts[cwe] else 0)
                   for cwe in total_counts}

    return per_file_results, avg_ratios, empty_rates


def write_csv(filepath, fieldnames, rows):
    """
    Write a list of dicts to a CSV file.
    """
    with open(filepath, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for row in rows:
            writer.writerow(row)


def main():
    parser = argparse.ArgumentParser(
        description="Pairwise compare txt files and output results to CSV, without pandas."
    )
    parser.add_argument('src_dir', help='Directory of source txt files')
    parser.add_argument('result_dir', help='Directory of processed txt files')
    parser.add_argument('-p', '--prefix', default='compression_report',
                        help='Prefix for output CSV files')
    args = parser.parse_args()

    if not os.path.isdir(args.src_dir) or not os.path.isdir(args.result_dir):
        print("Error: Both arguments must be valid directories.")
        return

    per_file, avg_ratios, empty_rates = analyze_pairwise(args.src_dir, args.result_dir)

    # Console output
    print("Per-file compression ratios:")
    for rec in per_file:
        if rec['CompressionRatio'] is not None:
            print(f"{rec['SourceFile']}: {rec['SourceLines']} -> {rec['ResultLines']} lines, ratio {rec['CompressionRatio']:.4f}")
        else:
            print(f"{rec['SourceFile']}: skipped (empty result or zero source lines)")

    print("\nAverage compression ratio by CWE:")
    for cwe, avg in avg_ratios.items():
        if avg is not None:
            print(f"{cwe}: {avg:.4f}")
        else:
            print(f"{cwe}: No valid data to compute average")

    print("\nEmpty result rate by CWE:")
    for cwe, rate in empty_rates.items():
        print(f"{cwe}: {rate:.2%} empty")

    # Prepare CSV rows
    per_file_csv = f"{args.prefix}_per_file.csv"
    avg_csv = f"{args.prefix}_average_by_cwe.csv"
    empty_csv = f"{args.prefix}_empty_rates.csv"

    write_csv(per_file_csv,
              ['SourceFile', 'ResultFile', 'CWE', 'SourceLines', 'ResultLines', 'CompressionRatio', 'IsEmptyResult'],
              per_file)

    write_csv(avg_csv,
              ['CWE', 'AverageCompressionRatio'],
              [{'CWE': cwe, 'AverageCompressionRatio': round(ratio, 4) if ratio is not None else ''}
               for cwe, ratio in avg_ratios.items()])

    write_csv(empty_csv,
              ['CWE', 'EmptyResultRate'],
              [{'CWE': cwe, 'EmptyResultRate': round(rate, 4)}
               for cwe, rate in empty_rates.items()])

    print("\nCSV reports generated:")
    print(f" - Per-file comparison: {per_file_csv}")
    print(f" - Average ratios by CWE: {avg_csv}")
    print(f" - Empty result rates: {empty_csv}")

if __name__ == '__main__':
    main()
