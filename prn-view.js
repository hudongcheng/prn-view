var net = require('net');
var fs = require('fs');

var server = net.createServer(function(c) { //'connection' listener
	console.log('------ Fax client connected ------');
	var filename = './' + Date.now() + '.prn';
	var fd = fs.openSync(filename, 'w');
	c.on('end', function() {
		console.log('------ Fax client disconnected ------');
/*		fs.writeFile('/tmp/pc-fax.tiff', tiff, function (err) {
			if (err) throw err;
				console.log('It\'s saved!');
			});*/
			fs.closeSync(fd);
	});

	c.on('data', function(data) {
		console.log(data);
		fs.writeSync(fd, data, 0, data.length);
	});
});

server.listen(9100, function() { //'listening' listener
	console.log('PRNView server bound');
});
