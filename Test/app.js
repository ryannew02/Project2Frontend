

var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
const cors = require('cors');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var app = express();

app.use(cors());

// Recompile cpp backend
const { execSync } = require('child_process');

try {
  console.log('--- Starting C++ Live Compilation via Makefile ---');
  execSync('make', { cwd: path.join(__dirname, 'Backend'), stdio: 'inherit' });
  console.log('--- C++ Compilation Successful ---');
} catch (compileError) {
  console.error('--- C++ Compilation Failed! ---', compileError);
}

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/users', usersRouter);



const { exec } = require('child_process');

// Create an API endpoint for React to fetch from
app.get('/api/random-array', (req, res) => {
  
  // Execute the compiled C++ program
  exec(path.join(__dirname, 'Backend', 'build', 'dataStructure.exe'), (error, stdout, stderr) => {
    if (error) {
      console.error(`Execution error: ${error}`);
      return res.status(500).send('Error running C++ code');
    }
    
    try {
      // stdout contains the printed output from C++
      // We parse the string "[5, 12...]" into an actual JavaScript array
      const dataArray = JSON.parse(stdout);
      
      // Send the array to React
      res.json(dataArray); 
    } catch (parseError) {
      res.status(500).send('Error parsing C++ output');
    }
  });
});

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
})

module.exports = app;
