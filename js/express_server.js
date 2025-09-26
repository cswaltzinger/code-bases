const express = require('express');
var app = express();

app.use(express.static('public'));
app.use(express.json())



app.get("/*",(req,res)=>{
    res.send("hello world")
})

app.listen(8080,()=>{
    console.log("server started at 8080")
})