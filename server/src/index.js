const net = require("net");

let server = net.createServer(socket => {
    console.log("[SOCKET] Connected.");

    socket.on("close", function () {
        console.log("[SOCKET] Disconnected.");
    });

    socket.on("data", function (data) {
        process.stdout.write(data.toString("utf8"));
    });
});

server.listen(8000, "127.0.0.1");
