const fs = require('fs');


function readFile(path){
    return fs.readFileSync(path,'utf-8')
}

function writeFile(path,data){
    fs.writeFileSync(path,data,'utf-8')
}

function getJson(path){
    let content = readFile(path)
    return JSON.parse(content)
}   

function saveJson(path,obj){
    let content = JSON.stringify(obj,null,2)
    writeFile(path,content)
}


