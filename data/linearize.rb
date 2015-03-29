#!/usr/bin/env ruby

dir = File.expand_path File.join(__FILE__, "..")

font = File.read(File.join(dir, "dos_spaced.bin")).chars.drop(0x36)

let = ->(n) {
  l=[];
  15.times do |m|
    l += font.drop((n/16)*384*15 + (n%16)*24 + m*384).take(24)
  end
  l
}

lin24 = []
lin8 = []
94.times do |n|
  l = let.(n)
  lin24 += l
  l.each_with_index { |c, i| lin8 << c if (i%3).zero? }
end

lin24file = File.open(File.join(dir, 'dos_lin_24.bin'), 'w') { |f| f.write lin24.join }
lin8file = File.open(File.join(dir, 'dos_lin_8.bin'), 'w') { |f| f.write lin8.join }
