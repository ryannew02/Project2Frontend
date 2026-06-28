const express = require('express');
const cors = require('cors');
const path = require('path');
const { spawn } = require('child_process');

const app = express();
const port = process.env.PORT || 3000;

app.use(cors({ origin: '*' }));
app.use(express.json());

//Debug landing page - identifies port, current time, and provides descriptions of available routes.
app.get('/', (req, res) => {
  res.send(`
    <html>
      <body style="font-family: monospace; padding: 30px; background: #EBF0F5; color: #2C353F;">
        <h2>Server is running</h2>
        <p><b>Port:</b> ${port}</p>
        <p><b>Time:</b> ${new Date().toLocaleString()}</p>
        <hr style="border-color: #4A5560;" />
        <h3>Available Routes</h3>
        <ul>
          <li><b>GET</b> <a href="/contributors">/contributors</a> — read up on the developers</li>
          <li><b>GET</b> <a href="/api/status">/api/status</a> — health check</li>
          <li><b>POST</b> /api/route — send addresses + passengers, returns route + distance</li>
        </ul>
        <hr style="border-color: #4A5560;" />
        <h3>C++ Binary</h3>
        <p><b>Path:</b> ${path.join(__dirname, 'Backend', 'build', 'dataStructure.exe')}</p>
      </body>
    </html>
  `);
});

//Contributors Page
app.get('/contributors', (req, res) => {
  res.send(`
    <html>
      <body style="font-family: monospace; padding: 30px; background: #EBF0F5; color: #2C353F;">
        <h1>Team Name: Zephyr</h1>
        <h2>Project Title: Vital Air Ways (VAW)</h2>
        <p><b>Last Updated on:</b> 06/28/2026</p>
        <p><b>Pre-Alpha 0.10</b></p>
        <hr style="border-color: #4A5560;" />
        <h3>Contributors</h3>
        <ul>
          <li><b>Ryan Juergens</li>
          <li><b>Timothy Arbo-Diehl</li>
          <li><b>Jon Tascon</li>
        </ul>
        <hr style="border-color: #4A5560;" />
        <h3>Component Ownership</h3>
        <p><b>Ryan Juergens - React State Management and Pipeline Architecture: </b>${path.join(__dirname, 'Backend', 'build', 'test.cpp')}</p>
        <p><b>Timothy Arbo-Diehl - Database Management and Analytics Engineering: </b>${path.join(__dirname, 'Backend', 'build', 'test.cpp')}</p>
        <p><b>Jon Tascon - Data Aggregation and Staging: </b>${path.join(__dirname, 'Backend', 'data', 'addresses.csv')}</p>
      </body>
    </html>
  `);
});

//Simple info about server status
app.get('/api/status', (req, res) => {
  res.json({
    status: 'ok',
    uptime: process.uptime().toFixed(2) + 's',
    time: new Date().toISOString(),
    port,
  });
});


//Start C++ Source Engine and prepare it for Pipeing
//builds file path and starts(spawn()) the source engine backround (proc)ess.
const binaryPath = path.join(__dirname, 'Backend', 'build', 'dataStructure.exe');
const cppProcess = spawn(binaryPath);
let outputBuffer = '';
cppProcess.stderr.on('data', (chunk) => {
  console.error('C++ stderr:', chunk.toString());
});
cppProcess.on('close', (code) => {
  console.error(`C++ process exited with code ${code}`);
});
console.log('C++ process started and data loaded');

//Primary Application Route - 
//This route takes the request body which is a JavaScipt object and converts it to a JSON that
//can be passed to the source engine 
app.post('/api/route', (req, res) => {
  //converts to c-source parsable json 
  const payload = JSON.stringify(req.body) + '\n';
  //debug line - prints the string sent to c-source to server console
  console.log('Sending to C++:', payload);

  // Listen for exactly one response then detach
  const onData = (chunk) => {
    outputBuffer += chunk.toString();

    // Wait until we get a complete JSON line
    const newlineIndex = outputBuffer.indexOf('\n');
    if (newlineIndex === -1) return;

    const responseLine = outputBuffer.slice(0, newlineIndex);
    outputBuffer = outputBuffer.slice(newlineIndex + 1);

    cppProcess.stdout.removeListener('data', onData);

    try {
      const parsed = JSON.parse(responseLine);
      parsed.algorithm = req.body.algorithm;
      res.json(parsed);
    } catch (e) {
      res.status(500).json({ error: 'Parse failed', raw: responseLine });
    }
  };

  cppProcess.stdout.on('data', onData);
  cppProcess.stdin.write(payload);
  });
//===============Need to document above^^^^


//This will run if any unregistered route is used
app.use((req, res) => {
  res.status(404).json({ error: `Route not found: ${req.method} ${req.path}` });
});

//Output to console to verify server started
app.listen(port, () => {
  console.log(`Server running on http://localhost:${port}`);
});

module.exports = app;