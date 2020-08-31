class Reaction
  attr_accessor :qty, :comp, :reqs

  def initialize(qty, comp, reqs)
    @qty = qty
    @comp = comp
    @reqs = reqs
  end

  def to_s
    parts = @reqs.map { |qty, comp| "#{qty} #{comp}" }.join(', ')
    "#{parts} => #{@qty} #{@comp}"
  end
end

def parse_component(string)
  _, qty, elem = string.match(/(\d+) (\w+)/).to_a
  qty = qty.to_i
  [qty, elem]
end

def read_input
  reactions = {}

  IO.readlines('input').each do |line|
    next if line.strip.empty?

    reqs, result = line.split('=>')
    result_qty, result_comp = parse_component(result)
    reqs = reqs.split(',').map { |part| parse_component(part) }

    if reactions.include?(result_comp)
      raise "Already found a reaction with result #{result}"
    end

    reactions[result_comp] = Reaction.new(result_qty, result_comp, reqs)
  end
  reactions
end

# Calculate how much ORE we need to generate the given amount of FUEL
def calc_ore(fuel)
  reactions = read_input

  # how much of each item we have on hand (from excess product from past
  # reactions)
  have = Hash.new 0

  # tracks how much of each item we still need to generate
  wishlist = Hash.new 0

  # initialize the wishlist with the requirements for FUEL
  reactions['FUEL'].reqs.each { |qty, comp| wishlist[comp] = qty * fuel }

  until wishlist.values.all?(&:zero?)
    comp, qty = wishlist.shift

    # how many times do we need to run this reaction to get what we need
    mult = (qty / reactions[comp].qty.to_f).ceil

    # add the excess from the reaction to our `have` buffer
    have[comp] += reactions[comp].qty * mult - qty
    reactions[comp].reqs.each do |req_qty, req|
      req_qty *= mult
      if req == 'ORE'
        have[req] += req_qty
        next
      end

      # if we have enough in our hands use that right away
      if req_qty <= have[req]
        req_qty = 0
        have[req] -= req_qty
      else
        req_qty -= have[req]
        have[req] = 0
      end

      # add new requirement to the running wishlist
      wishlist[req] += req_qty
    end
  end
  have['ORE']
end

def part1
  calc_ore(1)
end

def part2
  # Using the calc_ore() function, which can calculate the amount of ORE
  # needed for n units of FUEL in constant time, run a binary search between a
  # lower and an upper bound to find the right number of FUEL units we can
  # produce with the given amount of ORE.

  # amount of total ore to burn
  target = 1_000_000_000_000
  # ore per fuel
  ore = calc_ore(1)
  # the "expected" amount of total fuel is our lowest possible limit. only
  # reachable if we had zero left overs between iterations
  low = target / ore
  # we can never reach double the lower limit. that would mean we have enough
  # left overs to get iterations for free.
  high = low * 2

  # binary search
  while low <= high
    mid = low + (high - low) / 2
    distance = target - calc_ore(mid)
    if distance.abs < ore
      break distance.negative? ? mid - 1 : mid
    elsif distance > 0
      low = mid + 1
    else
      high = mid - 1
    end
  end
end

if $PROGRAM_NAME == __FILE__
  puts "Part 1: #{part1}"
  puts "Part 2: #{part2}"
end
