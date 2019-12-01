function setup() {
  noCanvas();
}

function mousePressed() {
  let myPlayState = {
    playState: 1
  };
  let url = 'http://10.23.11.246:8080';
  let content = JSON.stringify(myPlayState);
  httpPost(url, 'json', myPlayState, function(response) {
    print(response);
  });
  // httpDo(url, {
  //     method: 'GET',
  //     datatype: 'string',
  //   },
  //   function(res) {
  //     myPlayState = res;
  //   }
  // );
  // print(myPlayState);


}

function draw() {
  background(220);
}