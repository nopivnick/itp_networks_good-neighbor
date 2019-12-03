function setup() {
  //  noCanvas();
}

function mousePressed() {
  // let myPlayState = {
  //   playState: 1
  // };

  console.log("mouse clicked");
  let url = 'http://10.23.11.246:8080/playState/1';
  //let content = JSON.stringify(myPlayState);
  // httpPost(url, 'json', myPlayState, function(response) {
  //   print(response);
  // });

  // $.post("http://10.23.11.246:8080/playState/1", null, function(data, status) {
  //   console.log(`${data} and status ${status}`)
  // });
  httpDo(url, {
      method: 'POST',
    },
    function(res) {
      print("success");
    }
  );
  // print(myPlayState);



}

function draw() {
  background(220);
}