script_dir="$(cd "$(dirname "$0")" && pwd)"
echo "Script is located in: $script_dir"

cmake -Dtarget::STRING=${target} -S . -B build
cmake --build build