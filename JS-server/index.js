const WebSocket = require('ws');

const PORT = process.env.PORT || 3000;
const socket = new WebSocket('ws://localhost:' + PORT);

socket.on('open', () => {
  console.log('Ready to speak with the server');
});

socket.on('close', () => {
  console.log('server closed');
});

socket.on('error', (err) => {
  console.error(err);
});

socket.on('message', (buff) => {
  console.log('MESSAGE: ', buff.toString());
});

socket.on('upgrade', (req) => {
  console.log('UPGRADE REQUEST', req.headers);
});

process.stdin.on('data', (buff) => {
  socket.send(buff);
});

process.on('SIGINT', () => {
  socket.close();
  process.exit();
});
