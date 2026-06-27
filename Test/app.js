const express = require('express');
const cors = require('cors');
const path = require('path');
const { exec, spawn } = require('child_process');

const app = express();
const port = process.env.PORT || 3000;

app.use(cors({ origin: '*' }));
app.use(express.json());

// ─── Debug landing page ────────────────────────────────────────────────────
app.get('/', (req, res) => {
  res.send(`
    <html>
      <body style="font-family: monospace; padding: 30px; background: #969494; color: rgb(1, 65, 65);">
        <h2>Server is running</h2>
        <p><b>Port:</b> ${port}</p>
        <p><b>Time:</b> ${new Date().toLocaleString()}</p>
        <hr style="border-color: #333;" />
        <h3>Available Routes</h3>
        <ul>
          <li><a href="/api/random-array" style="color:#0ff;">/api/random-array</a> — runs C++ binary, returns float array</li>
          <li><a href="/api/status" style="color:#0ff;">/api/status</a> — server health check</li>
        </ul>
        <hr style="border-color: #333;" />
        <h3>C++ Binary</h3>
        <p><b>Expected path:</b> ${path.join(__dirname, 'Backend', 'build', 'dataStructure.exe')}</p>
      </body>
    </html>
  `);
});

// ─── Health check ──────────────────────────────────────────────────────────
app.get('/api/status', (req, res) => {
  res.json({
    status: 'ok',
    uptime: process.uptime().toFixed(2) + 's',
    time: new Date().toISOString(),
    port,
  });
});

// ─── Main data route ───────────────────────────────────────────────────────
//This calls the C source file 
app.get('/api/random-array', (req, res) => {
  const binaryPath = path.join(__dirname, 'Backend', 'build', 'dataStructure.exe');

  exec(binaryPath, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: 'Failed to run C++ binary', detail: error.message });
    }
    if (stderr) {
      console.warn(`C++ stderr: ${stderr}`);
    }

    try {
      const dataArray = JSON.parse(stdout);
      console.log(`Served ${dataArray.length} values`);
      res.json(dataArray);
    } catch (parseError) {
      console.error(`Parse error: ${parseError.message}`);
      res.status(500).json({ error: 'Failed to parse C++ output', detail: parseError.message });
    }
  });
});

//Testing communication
app.post('/api/route', (req, res) => {
  const payload = JSON.stringify(req.body);
  console.log('Sending to C++:', payload);

  const binaryPath = path.join(__dirname, 'Backend', 'build', 'dataStructure.exe');
  const proc = spawn(binaryPath);

  proc.stdin.write(payload);
  proc.stdin.end();

  let output = '';
  proc.stdout.on('data', (chunk) => { output += chunk; });
  proc.stderr.on('data', (chunk) => { console.error('C++ stderr:', chunk.toString()); });

proc.stdout.on('end', () => {
  console.log('C++ returned:', output);
  try {
    const parsed = JSON.parse(output);
    parsed.algorithm = req.body.algorithm;
    res.json(parsed);
  } catch (e) {
    res.status(500).json({ error: 'Parse failed', raw: output });
  }
});
});

// ─── 404 fallback ─────────────────────────────────────────────────────────
app.use((req, res) => {
  res.status(404).json({ error: `Route not found: ${req.method} ${req.path}` });
});

app.listen(port, () => {
  console.log(`Server running on http://localhost:${port}`);
});

module.exports = app;