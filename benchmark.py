import argparse
import hashlib
import json
from pathlib import Path
import platform
import re
import shutil
import statistics
import subprocess
import sys
import tempfile


def run_time(command, directory):
    output = subprocess.run(command, cwd=directory, capture_output=True, text=True, check=True).stdout
    match = re.search(r'execution time: ([\d.eE+-]+) microseconds', output)
    if not match:
        raise ValueError('Execution time missing from output')
    return float(match.group(1)) / 1000.0


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('csv', type=Path)
    parser.add_argument('executable', type=Path)
    parser.add_argument('--runs', type=int, default=21)
    parser.add_argument('--output', type=Path, default=Path('benchmark_results.json'))
    args = parser.parse_args()
    if args.runs < 3:
        parser.error('Use at least 3 measured runs')
    repo = Path(__file__).resolve().parent
    source = args.csv.resolve()
    commands = {'python': [sys.executable, str(repo / 'main.py')],
                'cpp': [str(args.executable.resolve())]}
    timings = {'python': [], 'cpp': []}
    with tempfile.TemporaryDirectory(dir=repo) as directory:
        shutil.copyfile(source, Path(directory) / 'aapl_data_2023-01-01_to_2026-06-01.csv')
        for _ in range(3):
            for command in commands.values():
                run_time(command, directory)
        # Alternate order to reduce systematic effects from always running one first.
        for i in range(args.runs):
            for name in (['python', 'cpp'] if i % 2 == 0 else ['cpp', 'python']):
                timings[name].append(run_time(commands[name], directory))
    result = {'dataset': source.name, 'sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
              'rows': len(source.read_text().splitlines()) - 1,
              'platform': platform.platform(), 'python_version': platform.python_version(),
              'warmups': 3, 'runs': args.runs, 'timings_ms': timings}
    for name, values in timings.items():
        result[name] = {'median_ms': statistics.median(values), 'mean_ms': statistics.mean(values),
                        'min_ms': min(values), 'max_ms': max(values)}
    result['speedup'] = result['python']['median_ms'] / result['cpp']['median_ms']
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))
