# rubocop:disable Style/GlobalVars, Metrics/AbcSize, Metrics/CyclomaticComplexity
require 'set'

def read_input
  $asteroids = Set.new
  IO.readlines('input').each_with_index do |line, y|
    line.strip.split('').each_with_index do |block, x|
      $asteroids << [x, y] if block == '#'
    end
  end
  $maxx = $asteroids.max_by { |x, _| x }[0]
  $maxy = $asteroids.max_by { |_, y| y }[1]
  $minx = $asteroids.min_by { |x, _| x }[0]
  $miny = $asteroids.min_by { |_, y| y }[1]
end

def simplify_fraction(numer, denom)
  # calculate greatest common denominator
  first = numer
  second = denom
  first, second = second, first % second while second.positive?
  gcd = first

  [numer / gcd, denom / gcd]
end

# blocks in line of sight between two asteroids
def lineofsight(block, other)
  # calculate the shape of the steps needed to move towards the objective as
  # pairs of x and y movements.
  xdist = (block[0] - other[0])
  ydist = (block[1] - other[1])
  # what is the minimum size of the step to move in that direction
  xstep, ystep = simplify_fraction(xdist.abs, ydist.abs)
  xstep = -xstep if xdist.positive?
  ystep = -ystep if ydist.positive?

  blocks = []
  move = block
  while move[0] <= $maxx && move[1] <= $maxy && move[0] >= $minx && move[1] >= $miny
    move = [move[0] + xstep, move[1] + ystep]
    blocks << move if $asteroids.include? move
  end
  blocks
end

def part1
  max = 0
  best = nil
  bestlos = nil
  $asteroids.each do |one|
    # count how many other blocks in can see
    count = 0
    blocked = Set.new
    lines = Set.new
    $asteroids.each do |other|
      next if one == other
      next if blocked.include? other

      los = lineofsight(one, other)
      lines << los
      count += 1 if los[0] == other
      blocked.merge los[1..]
    end
    if count > max
      max = count
      best = one
      bestlos = lines
    end
  end
  [best, max, bestlos]
end

# utility function to sort coordinates clock-wise
def compare(x, y)
  if x >= 0 && y <= 0
    y.zero? ? $maxx : x.to_f / -y
  elsif x >= 0 && y > 0
    ratio = x.zero? ? $maxx : y.to_f / x
    ratio + $maxx
  elsif x < 0 && y > 0
    (-x.to_f / y) + ($maxx * 2)
  else # if x < 0 && y <= 0
    (y.to_f / x) + ($maxx * 3)
  end
end

def part2(best, bestlos)
  # sort the list of lines of sight in clock-wise order relative to the
  # best location
  los = bestlos.to_a.sort do |a, b|
    a = [a.first[0] - best[0], a.first[1] - best[1]]
    b = [b.first[0] - best[0], b.first[1] - best[1]]
    compare(*a) <=> compare(*b)
  end

  # now that our lines of sight are sorted, vaporize them one by one until
  # we reach our desired count
  i = 0 
  vaporized = nil
  while i < 200
    los.each do |line|
      next if line.empty?

      vaporized = line.shift
      i+=1
      break if i == 200
    end
  end
  vaporized
end

# very pythonic, I know
if __FILE__ == $0
  read_input

  best, max, bestlos = part1
  puts "Best location #{best} can see #{max} asteroids"

  bet = part2(best, bestlos)
  ans = bet[0] * 100 + bet[1]
  puts "200th vaporization: #{bet}. Part 2 answer: #{ans}"
end
