//import express from "express"

const express = require('express')
const app = express()
const port = 3000

app.get('/', (req, res) => {
    res.send('Hello World again')
})

app.listen(port, () => {
    console.log('Server running ${port}')
})



//creating a path for your router (Path, run)
//C.R.U.D. commands POST, GET, PUT, DELETE 