#!/usr/bin/env ruby

require 'socket'

server = TCPServer.open(9100)

loop {
	Thread.start(server.accept) do |client|
		while line = client.gets
			puts line.chop
		end
		client.close
	end
}
