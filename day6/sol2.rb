require 'set'

input = IO.readlines('input')

# Store all the directly-accessible nodes in a dictionary
accessible = Hash.new { |k,v| k[v] = [] }
input.map { |x| x.strip }.each do |line|
  next if line.empty?
  key, value = line.split ')'
  # Key is accessible from value as value is accessible from key
  accessible[key] << value
  accessible[value] << key
end

# puts accessible

# Dijkstra's algorithm
start = "YOU"
finish = "SAN"
to_visit = [[start, 0]]
distances = Hash.new(Float::INFINITY)
seen = Set.new
while not to_visit.empty?
  node, distance = to_visit.shift
  accessible[node].each do |move|
    if distance + 1 < distances[move]
      distances[move] = distance + 1
    end
    if not seen.include? move and to_visit.none? {|x| x[0] == move}
      to_visit << [move, distance + 1]
    end
    seen << move
  end
end

# puts distances

sol = distances[finish] - 2  # -2 because we want to know the distance to the node santa orbits
puts "Distance to santa: #{sol}"
