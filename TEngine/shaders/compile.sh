shader=(`find . -type f -name \*.vert -o -name \*.frag`)
index=0

for i in "${shader[@]}"; do
    echo "Compiling $i.spv"
    glslc "$i" -o "$i.spv"
    index=$((index+1))
done