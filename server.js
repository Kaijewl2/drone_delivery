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

const go_to_location_script_path = './TOL_test/build/TOL_test';

app.use(cors({origin: 'http://127.0.0.1:5500'}));

app.get('/api/go_to_location', (req, res) => {
    // res.json automatically sets Content-Type to application/json
  execFile(go_to_location_script_path, ['arg1', 'arg2'], (error, stdout, stderr) => {
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

