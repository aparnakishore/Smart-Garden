var express = require('express');
var app = express();

app.get('/', function(req, res){
   res.send("Hello world!");
});


app.get('/listplants', function(req, res){
   res.send("Hello world!");
});

app.get('/waterML', function(req, res){
   res.send("Hello world!");
});

app.get('/light', function(req, res){
res.send("Hello world!");
});

app.get('/info', function(req, res){
res.send("Hello world!");
});


app.listen(3000);
