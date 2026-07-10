# Vital Air Ways (VAW)

## Overview
This repo runs a tunneled, locally-hosted middleware server (Express) backed by a C++ engine (Splay Tree + Trie), fronted by a React/Vite app deployed on Vercel.

## Requirements
- Windows with WSL2 installed
- Docker (running inside WSL / via Coolify)
- ngrok installed in WSL
- Node.js + npm

## Startup Procedure Publicly (Docker/Coolify + ngrok + Vercel)

## Start the pipeline
- Ensure Docker/Coolify is running.
- Start the Express middleware container: 
    currently g9fig8p8ft4arnkuvga1p242-211218665807 (run docker query [docker ps] if this to verify container ID)
    sudo docker start g9fig8p8ft4arnkuvga1p242-211218665807
- Expose the container port and coolify dashboard port to the public using grok config file.
    ngrok start --all --config /home/ryanj/.config/ngrok/ngrok.yml 
    (be sure to use your local container file path for the config file and ensure your authorization token is configured)
- Review the assigned domains and verify they are functional
## At this point you should be able to call the api directly using the a properly formatted JSON or continue setting up the frontend below
- Update the react fetch commands to call the newly exposed express server
- Go to Vercel.com or another react frontend hosting site and host the frontend that has now been pointed to the exposed backend 

## Startup Procedure Locally (Docker/Coolify + ngrok + Vercel)
- cd /Test test directory
- run npm install
- run node app.js
## In a seperate terminal
- cd / project directory
- run npm install
- run nom run dev

You can configure the port addresses but currently they are set up for 3000 and 5173 respectivly
http://localhost:3000/
http://localhost:5173/

## ==================Once Application is running=====================
## GUI should be self explanitory though some functionality is not entirely functional at this time. 

## Outside Acknowledgments & Attributions
## This project was initially bootstrapped using the standard [Vite React Template](https://vitejs.dev/) to set up the build pipeline, local development server, and base CSS variables (`index.css`), additionally an express server template was used to lay the framework for the middleware server.
## Third-party libraries and dependencies (such as React, Ngrok, and styling frameworks) were managed and installed via `npm` (Node Package Manager).