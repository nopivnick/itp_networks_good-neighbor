let myVolume = 0;
let neighbor;

let button;
let img;

function preload() {
  // Get current json from the server API
  let url = 'http://10.23.11.246:8080/volume';

  neighbor = loadJSON(url);
  img = loadImage('assets/comic.png')
}

function setup() {
  createCanvas(1920, 1080);
  image(img, 10, 10, 1290, 675);
  button = createButton('STOP MUSIC');
  let col = color(255, 20, 30);
  let white = color(255);

  button.style('background-color', col);
  button.style('color', white);
  button.style('borded', 0);
  button.style('font-size', '20px');
  button.style('font-family', 'roboto');
  button.position(970, 30);
  button.size(300, 230);
  button.mousePressed(makePost);
  //  noCanvas();
}

function makePost() {
  console.log("buttom clicked");
  let url = 'http://10.23.11.246:8080/playState/1';
  //let content = JSON.stringify(myPlayState);
  // httpPost(url, 'json', myPlayState, function(response) {
  //   print(response);
  // });

  httpDo(url, {
      method: 'POST',
    },
    function(res) {
      print(res);
    }
  );
  // print(myPlayState);

}

function draw() {

}
