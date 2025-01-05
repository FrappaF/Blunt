shopt -s nullglob
current_folder=$(dirname "$0")
files=("$current_folder"/../examples/*.blunt)
if [ ${#files[@]} -eq 0 ]; then
    echo "No .blunt files found in $(realpath "$current_folder")/examples"
    exit 1
fi

for file in "${files[@]}"; do
    echo "Running test: $file"
    if [ -f "$file" ]; then
        blunt "$file"
        echo "Done\n"
    else
        echo "Error opening file: $file"
    fi
done