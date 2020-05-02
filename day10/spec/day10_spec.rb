# rubocop:disable Metrics/BlockLength
require 'tempfile'
require 'rspec-parameterized'
require './day10'

describe 'Day 10' do
  context 'Part 1 Examples' do
    where(:input, :best, :max) do
      [
        [
          '.#..#
          .....
          #####
          ....#
          ...##',
          [3, 4], 8
        ],

        [
          '......#.#.
          #..#.#....
          ..#######.
          .#.#.###..
          .#..#.....
          ..#....#.#
          #..#....#.
          .##.#..###
          ##...#..#.
          .#....####',
          [5, 8], 33
        ],

        [
          '#.#...#.#.
          .###....#.
          .#....#...
          ##.#.#.#.#
          ....#.#.#.
          .##..###.#
          ..#...##..
          ..##....##
          ......#...
          .####.###.',
          [1, 2], 35
        ],

        [
          '.#..#..###
          ####.###.#
          ....###.#.
          ..###.##.#
          ##.##.#.#.
          ....###..#
          ..#.#..#.#
          #..#.#.###
          .##...##.#
          .....#.#..',
          [6, 3], 41
        ],

        [
          '.#..##.###...#######
          ##.############..##.
          .#.######.########.#
          .###.#######.####.#.
          #####.##.#.##.###.##
          ..#####..#.#########
          ####################
          #.####....###.#.#.##
          ##.#################
          #####.##.###..####..
          ..######..##.#######
          ####.##.####...##..#
          .#####..#.######.###
          ##...#.##########...
          #.##########.#######
          .####.#.###.###.#.##
          ....##.##.###..#####
          .#.#.###########.###
          #.#.#.#####.####.###
          ###.##.####.##.#..##',
          [11, 13], 210
        ]
      ]
    end

    before(:each) do
      @tmpfile = Tempfile.new
      FileUtils.move('input', @tmpfile)
      File.open('input', 'w') do |file|
        file << input
        file.close
      end
      read_input
    end

    after(:each) do
      FileUtils.move(@tmpfile, 'input')
    end

    with_them do
      it 'should match the example output' do
        gotbest, gotmax, = part1
        expect(gotbest).to eq best
        expect(gotmax).to eq max
      end
    end
  end
end
