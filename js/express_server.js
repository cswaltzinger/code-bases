const express = require('express');
var app = express();

app.use(express.static('public'));
app.use(express.json())
app.use(express.urlencoded({ extended: true }));
// app.use('/css', express.static(__dirname + 'public/css'))


app.get("/*",(req,res)=>{
    res.send("hello world")
})

app.listen(8080,()=>{
    console.log("server started at 8080")
})