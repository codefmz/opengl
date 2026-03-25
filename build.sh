if [ $# != 1 ];
then
    echo "Invalid argument!"
    exit 1
fi

script_dir="$(cd "$(dirname "$0")" && pwd)"
echo "Script is located in: $script_dir"

target=$1

echo "compile target - ${target} "

cmake -Dtarget::STRING=${target} -S . -B build
cmake --build build --target ${target}