mass() {
    fuel=$(echo "$1 / 3 - 2" | bc)
    total=0
    while [ "$fuel" -gt 0 ]; do
        total=$((total + fuel))
        fuel=$(echo "$fuel / 3 - 2" | bc)
    done
    echo $total
}

while read r; do
    mass "$r"
done < input | paste -s -d '+' | bc
