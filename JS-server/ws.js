const { WebSocketServer } = require('ws');

const ws = new WebSocketServer({
  port: 8080,
});

ws.on('listening', () => console.log('Listening on port 8080'));

ws.on('connection', (wsCtx, req) => {
  wsCtx.on('message', (msg) => {
    console.log('HEADERS: ', req.headers);
    console.log('MESSAGE:', msg.toString());
  });
  process.stdin.on('data', (buff) => {
    wsCtx.send(buff);
  });
});
