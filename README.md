## Right now this is setup to run virtually via a tunneled locally hosted middleware server with a test address database and a simplified backend c++ engine.

## In order to run this repo you will need to first clone the repo to your local drive
## =============FYI this repo is only suitiable for Windows systems at this time=================

## ============Running Locally=======================
## Prior to running the middleware server please be sure to configure the app.js file according to whether
## === it is to run locally or through a railroad/vercel pipeline, see notes in the source file
## Open up a terminal instance (CMDPROMT) navigate to the middleware server directory {repo root directory}\Test
## On first boot you will need to install dependencies so run "install npm"
## After needed dependancies are installed use the command "node app.js" to host the middleware server to http://localhost:3000

## Open up a second terminal instance (CMDPROMT) navigate to the frontend server directory {repo root directory}\
## On first boot you will need to install dependencies so run "install npm"
## After needed dependancies are installed use the command "npm run dev" to initialize the front end server
## The frontend server will host on http://localhost:5173/

## ==================Running through Docker/Coolify/ngrok/Vercel Pipeline=================
## Prior to running the middleware server please be sure to configure the app.js file according to whether
## === it is to run locally or through a railroad/vercel pipeline, see notes in the source file
## 
## You will need to ensure docker is running and coolify is started
## run the express server container currently g9fig8p8ft4arnkuvga1p242-225051354968 {sudo docker start g9fig8p8ft4arnkuvga1p242-225051354968}
## verify response from expected port curl http://localhost:3000
## open the ngrok tunnel using command {ngrok http 3000} in wsl
## verify the ngrok host url that is tunneling to local port 3000
## vercel app is static and should be able to access it as long as the ngrok host url has been unchanged 
## verify the fetch command in the app.jsx matches that host url from the ngrok tunnel
## the application should now have a clean pipeline between the local server and webserver

## ==================Once Application is running=====================
## GUI should be self explanitory though some functionality is not entirely functional at this time. 