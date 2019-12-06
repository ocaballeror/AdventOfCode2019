input = IO.readlines('input')

# Read the input by lines, split them by ) and store them in a Hash in reverse.
# This gives us a dictionary mapping each planet to the one they orbit.
$map = {}
input.map { |x| x.strip }.each do |line|
  next if line.empty?
  value, key = line.split ')'
  raise if $map.has_key? key
  $map[key] = value
end

# puts $map

# The orbits dictionary stores each planet and its number of recursive orbits around COM
$orbits = {"COM" => 0}

# Recursive function to give us the amount of orbits for a planet
def resolve_orbits(planet)
  planet = planet
  if $orbits.has_key? planet
    return $orbits[planet]
  else
    total = resolve_orbits($map[planet]) + 1
    $orbits[planet] = total
    return total
  end
end

# Resolve all the orbits for all the planets
$map.each_key do |key|
  resolve_orbits(key)
end

# puts $orbits

total = $orbits.values.sum
puts "Total orbits: #{total}"
