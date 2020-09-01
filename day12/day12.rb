XYZ = Struct.new(:x, :y, :z)

class Satellite
  attr_accessor :pos, :vel
  def initialize(x, y, z)
    @pos = XYZ.new(x, y, z)
    @vel = XYZ.new(0, 0, 0)
  end

  def cross(other)
    @vel.x += other.pos.x <=> @pos.x
    @vel.y += other.pos.y <=> @pos.y
    @vel.z += other.pos.z <=> @pos.z
  end

  def move
    @pos.x += @vel.x
    @pos.y += @vel.y
    @pos.z += @vel.z
  end

  def energy
    potential = @pos.x.abs + @pos.y.abs + @pos.z.abs
    kinetic = @vel.x.abs + @vel.y.abs + @vel.z.abs
    potential * kinetic
  end

  def to_s
    "pos=<x=#{@pos.x}, y=#{@pos.y}, z=#{@pos.z}>, vel=<x=#{@vel.x}, y=#{@vel.y}, z=#{@vel.z}>"
  end

  def clone
    s = Satellite.new(*@pos)
    s.vel = @vel.clone
    s
  end
end

def read_input
  satellites = []
  IO.readlines('input').each do |line|
    match = line.match(/x=(-?\d+), y=(-?\d+), z=(-?\d+)/)
    satellites << Satellite.new(*match.captures.map(&:to_i))
  end
  satellites
end

# apply gravity and velocity to every satellite in the list
def simulate(satellites)
  satellites.product(satellites).each do |one, other|
    next if other == one

    one.cross(other)
  end
  satellites.each(&:move)
end

# simulate 1000 times and return the total energy
def part1(satellites)
  1000.times do
    simulate satellites
  end
  # sum all the energies
  satellites.inject(0) { |sum, s| sum + s.energy }
end

def part2(satellites)
  # The idea here is to find the period of each coordinate i.e. the number of
  # steps that it takes us to get all the xs, ys or zs to be the same as the
  # beginning. Then we can calculate the least common multiple of the three to
  # get the combined period for all the coordinates

  initial = satellites.map(&:clone)
  periods = XYZ.new(0, 0, 0)
  steps = 0
  while periods.to_a.any?(&:zero?)
    steps += 1
    satellites = simulate(satellites)

    # for each of the coordinates, check if all satellites match with their
    # original state
    %i[x y z].each do |attr|
      next if periods.send(attr).positive?

      if satellites.zip(initial).all? { |a, b| a.pos.send(attr) == b.pos.send(attr) }
        periods.send("#{attr}=", steps + 1)
      end
    end
  end

  # calculate the least common divisor of all the periods
  periods.to_a.reduce(1, :lcm)
end

satellites = read_input
total = part1(satellites)
puts "(part 1) Total energy after steps: #{total}"

period = part2(read_input)
puts "(part 2) Period: #{period}"
