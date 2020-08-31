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

def debug(str)
  'proxy all the debug print calls for easy commenting/uncommenting'
  # puts str
end

def obtain(qty, comp, reactions, have)
  react = reactions[comp]
  debug react.to_s
  until have[comp] >= qty
    react.reqs.each do |req_qty, req_comp|
      if req_comp == 'ORE'
        debug "+#{req_qty} ORE"
        have['ORE'] += req_qty
        next
      end

      debug "have #{have}"
      debug "obtain #{req_qty} #{req_comp}"
      obtain(req_qty, req_comp, reactions, have)
      have[req_comp] -= req_qty
      debug "-#{req_qty} #{req_comp}"
    end
    debug "+#{react.qty} #{comp}"
    have[comp] += react.qty
  end
end

def part1
  have = Hash.new 0
  obtain(1, 'FUEL', read_input, have)
  puts "Part 1: #{have['ORE']} ORE"
end

def part2
  puts 'This takes a while...'
  reactions = read_input
  have = Hash.new 0
  first = obtain(1, 'FUEL', read_input, have)
  ore_per_fuel = have['ORE']
  ore_cycle = 0
  fuel_cycle = 0
  2.step.each do |it|
    obtain(it, 'FUEL', reactions, have)
    components = have.reject { |w| %w[FUEL ORE].include? w }
    next unless components.values.all?(&:zero?)

    ore_cycle = have['ORE']
    fuel_cycle = it
    break
  end

  puts "ore cycle: #{ore_cycle}"
  puts "fuel cycle: #{fuel_cycle}"

  target = 1_000_000_000_000
  have = Hash.new 0
  # because `obtain` increases ORE instead of reducing it, we need to invert the
  # value of ORE to know when we run out of it
  have['ORE'] = -(target % ore_cycle)
  have['FUEL'] = fuel_cycle * (target / ore_cycle)
  have['FUEL'].step.each do |it|
    obtain(it, 'FUEL', read_input, have)
    break if have['ORE'] > -ore_per_fuel
  end
  puts "Part 2: #{have['FUEL']} FUEL"
end

if $PROGRAM_NAME == __FILE__
  part1
  part2
end
