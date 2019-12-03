while read r; do
    echo "$r / 3 - 2" | bc
done < input | paste -s -d '+' | bc
