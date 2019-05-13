
var bbt = require('beebotte');

var bclient = new bbt.Connector({apiKey: 't9pcjkH4Xfft7DNrLnH6g1Ey', secretKey: 'ZzPLrhIM7wVEOwupbkQI4tLpbqjDjmy3'});

exports.handler = (event, context) => {

  try {

    if (event.session.new) {
      // New Session
      console.log("NEW SESSION")
    }

    switch (event.request.type) {

      case "LaunchRequest":
        // Launch Request
        console.log(`LAUNCH REQUEST`);
        context.succeed(
          generateResponse(
            buildSpeechletResponse("Welcome to my home", true),
            {}
          )
        )
      break;

      case "IntentRequest":
        // Intent Request
        console.log(`INTENT REQUEST`);
        var stateOn = 'on';
        var stateOff = 'off';
        var numOne = '1';
        var numTwo = '2';
        var light = 'light';
        var fan = 'fan';
        var second ='seconds';
        var minute = 'minutes';

        switch(event.request.intent.name){

          case "lightcontroller":
            var state = event.request.intent.slots.state.value;
            var num = event.request.intent.slots.num.value;

            if(state == stateOn && num == numOne){

                bclient.write({channel: 'myhome', resource: 'lightone', data: 0},
                	function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("light one is now on", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------light one on----------------------------------------

            }else if(state == stateOn && num == numTwo){

            
              bclient.write(
                  {channel: 'myhome', resource: 'lighttwo', data: 0},
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("light two is now on", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------light two on----------------------------------------

            }else if(state == stateOff && num == numOne){


              bclient.write(
                  {channel: 'myhome', resource: 'lightone', data: 1},
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("light one is now off", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------light one off----------------------------------------

            }else if(state == stateOff && num == numTwo){

             
              bclient.write(
                  {channel: 'myhome', resource: 'lighttwo', data: 1},
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("light two is now off", true),
                        {}
                    )
                    );
                  });

                 //----------------------------------------light two off----------------------------------------
            }
          break;// break of intent "lightcontroller"------------------------------------------------------------


          case 'fancontroller':
            var state = event.request.intent.slots.state.value;
            var num = event.request.intent.slots.num.value;
            if(state == stateOn && num == numOne){

              bclient.write(
                  {channel: 'myhome', resource: 'fanone', data: 0},
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("fan one is now on", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------fan one on----------------------------------------

            }else if(state == stateOn && num == numTwo){

              bclient.write(
                  {channel: 'myhome', resource: 'fantwo', data: 0},
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("fan two is now on", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------fan two on----------------------------------------

            }else if(state == stateOff && num == numOne){

              bclient.write(
                  {channel: 'myhome', resource: 'fanone', data: 1},
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("fan one is now off", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------fan one off----------------------------------------

            }else if(state == stateOff && num == numTwo){

              bclient.write(
                  {channel: 'myhome', resource: 'fantwo', data: 1},
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("fan two is now off", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------fan one off----------------------------------------
            }
          break;//break fan controller-------------------------------

          case 'devicesController':
            var state = event.request.intent.slots.state.value;
            
            if(state == stateOn){
              
              bclient.writeBulk(
                  {
                    channel: 'myhome',
                        records: [
                            { resource: 'lightone', data: 0 },
                            { resource: 'lighttwo', data: 0 },
                            { resource: 'fanone', data: 0 },
                            { resource: 'fantwo', data: 0 }
                            ]
                    },
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("all devices are on", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------all devices on----------------------------------------

            }else if(state == stateOff){

              bclient.writeBulk(
                  {
                    channel: 'myhome',
                        records: [
                            { resource: 'lightone', data: 1 },
                            { resource: 'lighttwo', data: 1 },
                            { resource: 'fanone', data: 1 },
                            { resource: 'fantwo', data: 1 }
                            ]
                    },
                  function(err, res) {
                    context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("all devices are off", true),
                        {}
                    )
                    );
                  });
                //----------------------------------------all devices off----------------------------------------

            }
          break;// break devices controller------------------------------------------------------------


          case 'Timer':
          var state = event.request.intent.slots.state.value;
          var device = event.request.intent.slots.device.value;
          var num = event.request.intent.slots.num.value;
          var time = event.request.intent.slots.time.value;
          var timeunit = event.request.intent.slots.timeunit.value;
          if(timeunit == minute && time > '60'){
          	context.succeed(
                      generateResponse( //Alexa response if successful
                        buildSpeechletResponse("Sorry, the maximum delay time is 60 minutes so please ask me again", false),
                        {}
                    )
                    );
          	
          }else{
          	if((state == stateOn || state == stateOff)&&(device == light || device == fan)&&(timeunit == second || timeunit == minute)&&(num == numOne || num == numTwo)){

						bclient.write({channel: 'myhome', resource: 'timer', data:device+'-'+num+'-'+state+'-'+time+'-'+timeunit},
	                  	function(err, res) {
	                    context.succeed(
	                      generateResponse( //Alexa response if successful
	                        buildSpeechletResponse("Your request is accepted", true),
	                        {}
	                    )
	                    );
	                  });

          	}else{

          		context.succeed(
                  generateResponse( //Alexa response if successful
                    buildSpeechletResponse("Sorry,Your request may be wrong, please try again", false),
                    {} ));
          	}

          
          } //end else


          break;//break ---- intent Timer---------------------------
            
        }
      break; //break ------ intent request------------------------

      case "SessionEndedRequest":
        // Session Ended Request
        console.log(`SESSION ENDED REQUEST`);
        break;

      default:
        context.fail(`INVALID REQUEST TYPE: ${event.request.type}`);

    }

  } catch(error) { context.fail(`Exception: ${error}`) }

}







// Helpers
buildSpeechletResponse = (outputText, shouldEndSession) => {

  return {
    outputSpeech: {
      type: "PlainText",
      text: outputText
    },
    shouldEndSession: shouldEndSession
  }

}

generateResponse = (speechletResponse, sessionAttributes) => {

  return {
    version: "1.0",
    sessionAttributes: sessionAttributes,
    response: speechletResponse
  }

}
