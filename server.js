/*const { createServer } = require('node:http');
const hostname = '127.0.0.1';
const port = 3000;

const server = createServer((req, res) => {
  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/plain');
  res.end('Hello World');
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});*/
const cors = require('cors');
const express = require('express');
const { execFile } = require('child_process');
const app = express();

const go_to_location_script_path = './go_to_location/build/go_to_location';

app.use(cors({origin: 'http://127.0.0.1:5500'}));
app.use(express.json());

app.get('/api/go_to_location', (req, res) => {

  const {lat, lng} = req.body;

  console.log(`got ${lat} , ${lng}`);

  // All logic in get(..) callback func runs when request made to path
  execFile(go_to_location_script_path, ['34.6767', '12.0139'], (error, stdout, stderr) => {
     if (error) {
        console.error(`Execution Error: ${error.message}`);
        return;
    }
    if (stderr) {
        console.error(`Standard Error: ${stderr}`);
        return;
    }
    console.log(`C++ Output:\n${stdout}`);
  })
  res.json({
        id: 42,
        name: "Tony Stark",
        role: "Genius, philanthropist, billionaire, playboy, not a painter"
    });
});

app.listen(3000, () => console.log('Server running on port 3000'));

