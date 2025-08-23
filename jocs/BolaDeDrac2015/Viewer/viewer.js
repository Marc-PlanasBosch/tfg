// *********************************************************************
// Global variables
// *********************************************************************

var randomNumber = 1;

// Viewer state
var gamePaused = true;
var gamePreview = false; //If true, render will be called for the next tick even if game is paused, and then will be set to false
var gameDirection = 1;
var gameAnim = true;
var actRound = 0; //Current round index

// Data
var raw_data_str; //String for storing the raw data
var dataLoaded = false; //Set to true when raw_data_str is ready to be parsed
var data = { } //Object for storing all the game data

// Animation
var speed = 100; //Ticks per second
var FRAMES_PER_ROUND = 4;
var frames = 0; //Incremented each tick, when it reaches FRAMES_PER_ROUND, actRound is updated (acording to gameDirection)

// Visuals
var unitSize = 0.6; // 1 = same size as tile
var unitLineWidth = 2;
var grid_color = "#888888";
var player_colors = {
    '0': "0000ea",
    '1': "efbb34",
    '2': "de1818",
    '3': "b507f5",
}

// *********************************************************************
// Utility functions
// *********************************************************************


function getURLParameter (name) {
    // http://stackoverflow.com/questions/1403888/get-url-parameter-with-jquery
    var a = (RegExp(name + '=' + '(.+?)(&|$)').exec(location.search)||[,null])[1]
    if (a != null) return decodeURI(a);
    return null;
}

//Callback has a single parameter with the file contents
function loadFile (file, callback) {

    var xmlhttp;

    if (file == null || file == "") {
        alert("You must specify a file to load");
        return;
    }

    if (window.XMLHttpRequest) {
        // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    } else {
        // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }

    // http://www.w3schools.com/ajax/ajax_xmlhttprequest_onreadystatechange.asp
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState == 4) {
            //Note: We can not check xmlhttp.status != 200 for errors because status is not set when loading local files
            callback(xmlhttp.responseText);
        }
    }

    xmlhttp.open("GET", file, false);
    xmlhttp.send();
}

function int (s) {
    return parseInt(s);
}

function double (s) {
    return parseFloat(s);
}


function parse_assert(read_value, expected_value) {
    var correct = (read_value == expected_value);
    if (!correct) alert("Error parsing file, expected token: " + expected_value + ", read token: " + read_value);
    return correct;
}

// *********************************************************************
// Initialization functions
// *********************************************************************

function parseData (raw_data_str) {

    data.tileSize = 50;

    if ("" == raw_data_str) {
        alert("Could not load game file");
        return false;
    }

    // convert text to tokens
    var st = raw_data_str + "";
    var t = st.replace('\n', ' ').split(/\s+/);
    var p = 0;

    data.secgame = (t[p++] == "SecGame");

    s = t[p++];
    data.version = t[p++];
    if (s != "boladedrac") alert("Error at round " + round + ".\n" + "boladedrac");
    if (data.version != "v1") alert("Error at round " + round + ".\n" + "version");

    s = t[p++];
    data.nb_players = int(t[p++]);
    if (s != "nb_players") alert("Error at round " + round + ".\n" + "nb_players");

    s = t[p++];
    data.nb_rounds = int(t[p++]);
    if (s != "nb_rounds") alert("Error at round " + round + ".\n" + "nb_rounds");

    s = t[p++];
    data.nb_capsules = int(t[p++]);
    if (s != "nb_capsules") alert("Error at round " + round + ".\n" + "capsules");

    s = t[p++];
    data.nb_balls = int(t[p++]);
    if (s != "nb_balls") alert("Error at round " + round + ".\n" + "balls");

    s = t[p++];
    data.nb_beans = int(t[p++]);
    if (s != "nb_beans") alert("Error at round " + round + ".\n" + "beans");

    s = t[p++];
    data.nb_kintons = int(t[p++]);
    if (s != "nb_kintons") alert("Error at round " + round + ".\n" + "kintons");

    s = t[p++];
    data.goku_regen_time = int(t[p++]);
    if (s != "goku_regen_time") alert("Error at round " + round + ".\n" + "goku_regen_time");

    s = t[p++];
    data.bean_regen_time = int(t[p++]);
    if (s != "bean_regen_time") alert("Error at round " + round + ".\n" + "bean_regen_time");

    s = t[p++];
    data.kinton_regen_time = int(t[p++]);
    if (s != "kinton_regen_time") alert("Error at round " + round + ".\n" + "kinton_regen_time");

    s = t[p++];
    data.kinton_life_time = int(t[p++]);
    if (s != "kinton_life_time") alert("Error at round " + round + ".\n" + "kinton_life_time");

    s = t[p++];
    data.max_strength = int(t[p++]);
    if (s != "max_strength") alert("Error at round " + round + ".\n" + "max_strength");

    s = t[p++];
    data.res_strength = int(t[p++]);
    if (s != "res_strength") alert("Error at round " + round + ".\n" + "res_strength");

    s = t[p++];
    data.moving_penalty = int(t[p++]);
    if (s != "moving_penalty") alert("Error at round " + round + ".\n" + "moving_penalty");

    s = t[p++];
    data.kamehame_penalty = int(t[p++]);
    if (s != "kamehame_penalty") alert("Error at round " + round + ".\n" + "kamehame_penalty");

    s = t[p++];
    data.combat_penalty = int(t[p++]);
    if (s != "combat_penalty") alert("Error at round " + round + ".\n" + "combat_penalty");

    s = t[p++];
    data.rows = int(t[p++]);
    if (s != "rows") alert("Error at round " + round + ".\n" + "rows");

    s = t[p++];
    data.cols = int(t[p++]);
    if (s != "cols") alert("Error at round " + round + ".\n" + "cols");

    data.goku = new Array();
    for (var i = 0; i < data.nb_players; ++i) {
        data.goku[i] = new Object();
    }

    s = t[p++];
    if (s != "names") alert("Error at round " + round + ".\n" + "names");
    data.names = new Array();
    for (var i = 0; i < data.nb_players; ++i) {
        data.names[i] = t[p++];
    }

    data.rounds = new Array();
    for (var round = 0; round <= data.nb_rounds; ++round) {

        $("#debug").html(round);

        if (t[p++] != "round") alert("Error at round " + round + ".\n" + "round");
        if (int(t[p++]) != round) alert("Error at round " + round + ".\n" + "wrong round");

        // maze
        data.rounds[round] = new Object();
        data.rounds[round].rows = new Array();
        for (var i = 0; i < data.rows; ++i)
            data.rounds[round].rows[i] = t[p++];

        // beans
        if (t[p++] != "beans") alert("Error at round " + round + ".\n" + "beans");
	data.rounds[round].beans = new Array();
        for (var k = 0; k < data.nb_beans; ++k) {
	    data.rounds[round].beans[k] = new Object();
            data.rounds[round].beans[k].pos_x   = int(t[p++]);
            data.rounds[round].beans[k].pos_y   = int(t[p++]);
            data.rounds[round].beans[k].present =     t[p++];
            data.rounds[round].beans[k].time    = int(t[p++]);
        }

        // kintons
        if (t[p++] != "kintons") alert("Error at round " + round + ".\n" + "kintons");
	data.rounds[round].kintons = new Array();
        for (var k = 0; k < data.nb_kintons; ++k) {
	    data.rounds[round].kintons[k] = new Object();
            data.rounds[round].kintons[k].pos_x   = int(t[p++]);
            data.rounds[round].kintons[k].pos_y   = int(t[p++]);
            data.rounds[round].kintons[k].present =     t[p++];
            data.rounds[round].kintons[k].time    = int(t[p++]);
        }

        // gokus
        data.rounds[round].team = new Array();
        data.rounds[round].cpu  = new Array();
        for (var i = 0; i < data.nb_players; ++i) {
            s = t[p++];
            if (s != "normal"    &&
		s != "on_kinton" &&
		s != "with_ball" &&
		s != "on_kinton_with_ball") alert("Error at round " + round + ".\n" + "goku");

            data.rounds[round].team[i] = new Object();
            data.rounds[round].team[i].pos_x    =    int(t[p++]);
            data.rounds[round].team[i].pos_y    =    int(t[p++]);
            data.rounds[round].team[i].time     =    int(t[p++]);
            data.rounds[round].team[i].state    =        t[p++];
            data.rounds[round].team[i].balls    =    int(t[p++]);
            data.rounds[round].team[i].strength =    int(t[p++]);
            data.rounds[round].team[i].kinton   =    int(t[p++]);
            data.rounds[round].cpu[i]           = double(t[p++]);

            if (s == "on_kinton" || s == "on_kinton_with_ball") data.rounds[round].team[i].on_kinton = true;
	    else                                                data.rounds[round].team[i].on_kinton = false;

            if (s == "with_ball" || s == "on_kinton_with_ball") data.rounds[round].team[i].with_ball = true;
	    else                                                data.rounds[round].team[i].with_ball = false;
	}

        if (round != data.nb_rounds) {
            // actions asked
            if (t[p++] != "actions_asked") alert("Error at round " + round + ".\n" + "no actions_asked");
            for (var i = 0; i < data.nb_players; ++i) {
                if (int(t[p++]) != i) alert("Error at round " + round + ".\n" + "wrong player");
		// ignore
                t[p++];
                t[p++];
            }

            // actions done
            if (t[p++] != "actions_done") alert("Error at round " + round + ".\n" + "no actions_done");
            for (var i = 0; i < data.nb_players; ++i) {
                if (int(t[p++]) != i) alert("Error at round " + round + ".\n" + "wrong player");
                data.rounds[round].team[i].action_type = t[p++];
                data.rounds[round].team[i].action_dir  = t[p++];
            }
	}
    }

    return true;
}


//Initializing the game
function initGame (raw_data) {

    document.getElementById("loadingdiv").style.display="";

    //TODO: Next two calls could run concurrently
    if (parseData(raw_data) === false) return;
    preloadImages();

    // prepare state variables
    /*if (getURLParameter("start") == "yes") gamePaused = false;
      else gamePaused = true;*/
    gamePaused = false;

    gamePreview = true;

    // slider init
    $("#slider").slider({
	min: 0,
	max: data.nb_rounds,
    });

    // Canvas element
    canvas = document.getElementById('myCanvas');
    context = canvas.getContext("2d");

    // prepare the slider
    $("#slider").slider({
        slide: function(event, ui) {
            var value = $("#slider").slider( "option", "value" );
            actRound = value;
            frames = 0;
            gamePaused = true;
            gamePreview = true;
        }
    });
    $("#slider").width(500);

    // set the listerners for interaction
    document.addEventListener('mousewheel', onDocumentMouseWheel, false);
    document.addEventListener('keydown', onDocumentKeyDown, false);
    document.addEventListener('keyup', onDocumentKeyUp, false);

    window.addEventListener('resize', onWindowResize, false);
    onWindowResize();

    document.getElementById("loadingdiv").style.display="none";
    document.getElementById("gamediv").style.display="";

    mainloop();
}


function preloadImages () {

    data.img = new Array();

    // grass
    data.img.tile_grass0 = new Image();
    data.img.tile_grass0.src = "img/tile_grass0.png";
    data.img.tile_grass1 = new Image();
    data.img.tile_grass1.src = "img/tile_grass1.png";
    data.img.tile_grass2 = new Image();
    data.img.tile_grass2.src = "img/tile_grass2.png";
    data.img.tile_grass3 = new Image();
    data.img.tile_grass3.src = "img/tile_grass3.png";
    data.img.tile_grass4 = new Image();
    data.img.tile_grass4.src = "img/tile_grass4.png";

    // floor
    data.img.tile_floor = new Image();
    data.img.tile_floor.src = "img/tile_floor.png";

    // rocks
    data.img.tile_rock = new Image();
    data.img.tile_rock.src = "img/tile_rock.png";
    data.img.tile_rock1 = new Image();
    data.img.tile_rock1.src = "img/tile_rock1.png";
    data.img.tile_rock2 = new Image();
    data.img.tile_rock2.src = "img/tile_rock2.png";
    data.img.tile_rock3 = new Image();
    data.img.tile_rock3.src = "img/tile_rock3.png";
    data.img.tile_rock4 = new Image();
    data.img.tile_rock4.src = "img/tile_rock4.png";
    data.img.tile_rock5 = new Image();
    data.img.tile_rock5.src = "img/tile_rock5.png";
    data.img.tile_rock6 = new Image();
    data.img.tile_rock6.src = "img/tile_rock6.png";
    data.img.tile_rock7 = new Image();
    data.img.tile_rock7.src = "img/tile_rock7.png";
    data.img.tile_rock8 = new Image();
    data.img.tile_rock8.src = "img/tile_rock8.png";
    data.img.tile_rock9 = new Image();
    data.img.tile_rock9.src = "img/tile_rock9.png";
    data.img.tile_rock10 = new Image();
    data.img.tile_rock10.src = "img/tile_rock10.png";
    data.img.tile_rock11 = new Image();
    data.img.tile_rock11.src = "img/tile_rock11.png";
    data.img.tile_rock12 = new Image();
    data.img.tile_rock12.src = "img/tile_rock12.png";
    data.img.tile_rock13 = new Image();
    data.img.tile_rock13.src = "img/tile_rock13.png";
    data.img.tile_rock14 = new Image();
    data.img.tile_rock14.src = "img/tile_rock14.png";
    data.img.tile_rock15 = new Image();
    data.img.tile_rock15.src = "img/tile_rock15.png";
    data.img.tile_rock16 = new Image();
    data.img.tile_rock16.src = "img/tile_rock16.png";

    // objects
    data.img.tile_bean = new Image();
    data.img.tile_bean.src = "img/bean.png";
    data.img.tile_capsule = new Image();
    data.img.tile_capsule.src = "img/capsule.png";
    data.img.tile_ball = new Image();
    data.img.tile_ball.src = "img/ball.png";
    data.img.tile_kinton = new Image();
    data.img.tile_kinton.src = "img/kinton.png";

    // gokus without kinton
    data.img.spr_goku = new Array();
    for (var i = 0; i < data.nb_players; ++i) {
        data.img.spr_goku[i] = new Image();
        data.img.spr_goku[i].src = "img/goku-"+player_colors[i]+".png";
    }

    // gokus with kinton
    data.img.spr_kinton = new Array();
    data.img.spr_kinton = new Array();
    for (var i = 0; i < data.nb_players; ++i) {
        data.img.spr_kinton[i] = new Image();
        data.img.spr_kinton[i].src = "img/goku-kinton-"+player_colors[i]+".png";
    }

    // kamehame
    data.img.spr_kamehame_ini_b2t = new Image();
    data.img.spr_kamehame_ini_b2t.src = "img/kamehame-ini-b2t.png";
    data.img.spr_kamehame_ini_t2b = new Image();
    data.img.spr_kamehame_ini_t2b.src = "img/kamehame-ini-t2b.png";
    data.img.spr_kamehame_ini_l2r = new Image();
    data.img.spr_kamehame_ini_l2r.src = "img/kamehame-ini-l2r.png";
    data.img.spr_kamehame_ini_r2l = new Image();
    data.img.spr_kamehame_ini_r2l.src = "img/kamehame-ini-r2l.png";

    data.img.spr_kamehame_fin_b2t = new Image();
    data.img.spr_kamehame_fin_b2t.src = "img/kamehame-fin-b2t.png";
    data.img.spr_kamehame_fin_t2b = new Image();
    data.img.spr_kamehame_fin_t2b.src = "img/kamehame-fin-t2b.png";
    data.img.spr_kamehame_fin_l2r = new Image();
    data.img.spr_kamehame_fin_l2r.src = "img/kamehame-fin-l2r.png";
    data.img.spr_kamehame_fin_r2l = new Image();
    data.img.spr_kamehame_fin_r2l.src = "img/kamehame-fin-r2l.png";

    data.img.spr_kamehame_v = new Image();
    data.img.spr_kamehame_v.src = "img/kamehame-v.png";
    data.img.spr_kamehame_h = new Image();
    data.img.spr_kamehame_h.src = "img/kamehame-h.png";
}


// *********************************************************************
// Main loop functions
// *********************************************************************

function updateGame () {
    $("#slider").slider("option", "value", actRound);
}


function writeGameState () {
    // write round
    $("#round").html("Round: " + actRound);

    //update scoreboard
    var scoreboard = "";

    for (var i = 0; i <= 2; ++i)
    	scoreboard += "<br/><br/>";

    scoreboard += "<td style='background-color:#999999;'>";

    for (var i = 0; i < data.nb_players; i++) {
        scoreboard += "<span class='score'>"
            + "<div style='display:inline-block; margin-top: 5px; width:20px; height:20px; background-color:#"+ player_colors[i] +"'></div>"
            + "<div style='display:inline-block; vertical-align: middle; margin-bottom: 7px; margin-left:8px;'>"+data.names[i]+"</div>"
            + "<br/>"
            + "<div style='margin-left: 10px;'>"
            + "<div style='padding:2px;'>Score: "+ (data.rounds[actRound].team[i].balls * (1 + data.max_strength) + data.rounds[actRound].team[i].strength) +"</div>"
            + (data.secgame? "<div style='padding:2px;'>CPU: " + (data.rounds[actRound].cpu[i] == -1 ? " <font color=\"red\"><b>OUT</b></font>" : int(1000*data.rounds[actRound].cpu[i])/10. + "%") + "</div>" : "")
            + "</div>"
            + "</span><br/><br/><br/>";
    }
    $("#scores").html(scoreboard);
}

function drawGame () {

    if (canvas.getContext) {
        var context = canvas.getContext('2d');
        var rectSize = data.tileSize;

	var fixedMargin = 60;
	if (double(data.rows)/double(data.cols) > 0.9) {
	    fixedMargin = 10;
	}
	var heightScore = 180;

	canvas.width  = window.innerWidth  - 2*fixedMargin;
        canvas.height = window.innerHeight - 2*fixedMargin - heightScore;

	var sw = canvas.width  /(rectSize*data.cols);
	var sh = canvas.height/(rectSize*data.rows);
	var s;
	if (sw < sh) {
	    s = sw;
	    var offset = (canvas.height - s*rectSize*data.rows)/ 2;
	    canvas.style.marginTop  = fixedMargin + offset;
	    canvas.style.marginLeft = fixedMargin;
	}
	else {
	    s = sh;
	    var offset = (canvas.width - s*rectSize*data.cols)/ 2;
	    canvas.style.marginTop  = fixedMargin;
	    canvas.style.marginLeft = fixedMargin + offset;
	}
        context.scale(s, s);

        // outer rectangle
        context.fillStyle = "rgb(0,0,0)";
        context.fillRect(0, 0, rectSize*data.cols, rectSize*data.rows);

	randomNumber = 1;
        // draw maze
        for (var i = 0; i < data.rows; i++)
            drawRow(actRound, i);



	// draw beans and kintons
        for (var k = 0; k < data.nb_beans; ++k)
	    if (data.rounds[actRound].beans[k].present == 'y') {
                var x = data.rounds[actRound].beans[k].pos_x;
                var y = data.rounds[actRound].beans[k].pos_y;
                var img = data.img.tile_bean;
                var ctx = canvas.getContext('2d');
                ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
	    }
        for (var k = 0; k < data.nb_kintons; ++k)
	    if (data.rounds[actRound].kintons[k].present == 'y') {
                var x = data.rounds[actRound].kintons[k].pos_x;
                var y = data.rounds[actRound].kintons[k].pos_y;
                var img = data.img.tile_kinton;
                var ctx = canvas.getContext('2d');
                ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
	    }

        // draw gokus
	var kamehame_pos_x = -1;
	var kamehame_pos_y = -1;
	var kamehame_dir = 'n';

        for (var i = 0; i < data.nb_players; i++) {

            if (data.rounds[actRound].team[i].state == 'a') {

                if (! gameAnim) {
                    var x = data.rounds[actRound].team[i].pos_x;
                    var y = data.rounds[actRound].team[i].pos_y;
                    var ctx = canvas.getContext('2d');
		    if (data.rounds[actRound].team[i].on_kinton) {
			var img = data.img.spr_kinton[i];
			ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
		    }
		    else {
			var img = data.img.spr_goku[i];
			ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
		    }
                } else {
		    if (data.rounds[actRound].team[i].action_type == 't') {
			kamehame_pos_x = data.rounds[actRound].team[i].pos_x;
			kamehame_pos_y = data.rounds[actRound].team[i].pos_y;
			kamehame_dir   = data.rounds[actRound].team[i].action_dir;
		    }
                    var x;
		    if (data.rounds[actRound].team[i].on_kinton) {
			x = data.rounds[actRound].team[i].pos_x;
			x = x*data.tileSize;
			if (data.rounds[actRound].team[i].action_type !='t')
                            x = setMovedFastX(x, data.rounds[actRound].team[i].action_dir);
		    }
		    else {
			var tmpRound = actRound;            // gokus without kinton can only move in even rounds
			if (actRound % 2 == 1) --tmpRound;
			x = data.rounds[tmpRound].team[i].pos_x;
			x = x*data.tileSize;
			if (data.rounds[tmpRound].team[i].action_type !='t')
			    x = setMovedX(x, data.rounds[tmpRound].team[i].action_dir);
		    }

                    var y;
		    if (data.rounds[actRound].team[i].on_kinton) {
			y = data.rounds[actRound].team[i].pos_y;
			y = y*data.tileSize;
			if (data.rounds[actRound].team[i].action_type !='t')
                            y = setMovedFastY(y, data.rounds[actRound].team[i].action_dir);
		    }
		    else {
			var tmpRound = actRound;            // gokus without kinton can only move in even rounds
			if (actRound % 2 == 1) --tmpRound;
			y = data.rounds[tmpRound].team[i].pos_y;
			y = y*data.tileSize;
			if (data.rounds[tmpRound].team[i].action_type !='t')
			    y = setMovedY(y, data.rounds[tmpRound].team[i].action_dir);
		    }

                    var ctx = canvas.getContext('2d');

		    // gokus with ball are surrounded by a small thick circle
                    if (data.rounds[actRound].team[i].with_ball) {
			ctx.beginPath();
			ctx.strokeStyle = "#" + player_colors[i];
			ctx.lineWidth = 5;
			ctx.arc(y+25, x+25, 32, 0, Math.PI*2, true);
			ctx.closePath();
			ctx.stroke();
                    }

		    // regenerated gokus are surrounded by a big thin circle
                    if (actRound > 6 && data.rounds[actRound - 6].team[i].state == 'd') {
                        ctx.beginPath();
			ctx.strokeStyle = "#" + player_colors[i];
                        ctx.lineWidth = 2;
                        ctx.arc(y+25, x+25, 64, 0, Math.PI*2, true);
                        ctx.closePath();
                        ctx.stroke();
                    }

		    if (data.rounds[actRound].team[i].on_kinton) {
			var img = data.img.spr_kinton[i];
			ctx.drawImage(img, y, x);
		    }
		    else {
			var img = data.img.spr_goku[i];
			ctx.drawImage(img, y, x);
		    }
                }
            } else {

		// regenerated gokus are surrounded by a big thin circle
                if (actRound <= data.nb_rounds - 6 && data.rounds[actRound + 6].team[i].state == 'a') {

                    var r = 6;
                    while (data.rounds[actRound + r].team[i].state != 'd') r--;
                    r++;

                    var x = data.rounds[actRound + r].team[i].pos_x;
                    x = x*data.tileSize;

                    var y = data.rounds[actRound + r].team[i].pos_y;
                    y = y*data.tileSize;

                    var ctx = canvas.getContext('2d');

                    ctx.beginPath();
                    ctx.strokeStyle = "#" + player_colors[i];
                    ctx.lineWidth = 2;
                    ctx.arc(y+25, x+25, 64-4*r, 0, Math.PI*2, true);
                    ctx.closePath();
                    ctx.stroke();
                }
            }
        }

	// draw kamehames
       if (frames < (FRAMES_PER_ROUND/2))
	    drawKamehame(kamehame_pos_x, kamehame_pos_y, kamehame_dir);
    }
}

function drawKamehame(pos_x, pos_y, dir) {

    if (dir == 'n') return;

    var x = pos_x;
    var y = pos_y;

    if      (dir == 't') --x;
    else if (dir == 'b') ++x;
    else if (dir == 'l') --y;
    else if (dir == 'r') ++y;

    // do not show kamehames that are degenerate (== there is no room)
    if (data.rounds[actRound].rows[x][y] == 'X') return;

    var ctx = canvas.getContext('2d');


    // paint initial part of kamehame
    if      (dir == 't') img = data.img.spr_kamehame_ini_b2t;
    else if (dir == 'b') img = data.img.spr_kamehame_ini_t2b;
    else if (dir == 'l') img = data.img.spr_kamehame_ini_r2l;
    else if (dir == 'r') img = data.img.spr_kamehame_ini_l2r;
    ctx.drawImage(img, y*data.tileSize, x*data.tileSize);

    if      (dir == 't') --x;
    else if (dir == 'b') ++x;
    else if (dir == 'l') --y;
    else if (dir == 'r') ++y;

    // using that the board is surrounded by rocks
    while (data.rounds[actRound].rows[x][y] != 'X') {

	// paint middle part of kamehame
	if (dir == 't' || dir == 'b') img = data.img.spr_kamehame_v;
	else                          img = data.img.spr_kamehame_h;
	ctx.drawImage(img, y*data.tileSize, x*data.tileSize);

	if      (dir == 't') --x;
	else if (dir == 'b') ++x;
	else if (dir == 'l') --y;
	else if (dir == 'r') ++y;
    }

    // paint final part of kamehame
    if      (dir == 't') img = data.img.spr_kamehame_fin_b2t;
    else if (dir == 'b') img = data.img.spr_kamehame_fin_t2b;
    else if (dir == 'l') img = data.img.spr_kamehame_fin_r2l;
    else if (dir == 'r') img = data.img.spr_kamehame_fin_l2r;
    ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
}

//Note: X and Y of the next functions are reversed
function setMovedX (x, action) {
    if (action == 'n') return x;
    if (action == 't') {
        return (x - (((frames + FRAMES_PER_ROUND * (actRound % 2))*data.tileSize)/(2*FRAMES_PER_ROUND)));
    }
    if (action == 'b') {
        return (x + (((frames + FRAMES_PER_ROUND * (actRound % 2))*data.tileSize)/(2*FRAMES_PER_ROUND)));
    }
    return x;
}

function setMovedY (y, action) {
    if (action == 'n') return y;
    if (action == 'l')
        return (y - (((frames + FRAMES_PER_ROUND * (actRound % 2))*data.tileSize)/(2*FRAMES_PER_ROUND)));
    if (action == 'r')
        return (y + (((frames + FRAMES_PER_ROUND * (actRound % 2))*data.tileSize)/(2*FRAMES_PER_ROUND)));
    return y;
}

function setMovedFastX (x, action) {
    if (action == 'n') return x;
    if (action == 't') {
        return (x - ((frames*data.tileSize)/FRAMES_PER_ROUND));
    }
    if (action == 'b') {
        return (x + ((frames*data.tileSize)/FRAMES_PER_ROUND));
    }
    return x;
}

function setMovedFastY (y, action) {
    if (action == 'n') return y;
    if (action == 'l')
        return (y - ((frames*data.tileSize)/FRAMES_PER_ROUND));
    if (action == 'r')
        return (y + ((frames*data.tileSize)/FRAMES_PER_ROUND));
    return y;
}

function drawRow (round, row) {
    var ctx = canvas.getContext('2d');
    var rectSize = data.tileSize;
    for (var i = 0; i < data.cols; ++i) {
	randomNumber = (125 * randomNumber + 1) % 4096;
	switch (randomNumber % 5) {
	case 0: var img = data.img.tile_grass0; ctx.drawImage(img, i*rectSize, row*rectSize); break;
	case 1: var img = data.img.tile_grass1; ctx.drawImage(img, i*rectSize, row*rectSize); break;
	case 2: var img = data.img.tile_grass2; ctx.drawImage(img, i*rectSize, row*rectSize); break;
	case 3: var img = data.img.tile_grass3; ctx.drawImage(img, i*rectSize, row*rectSize); break;
	case 4: var img = data.img.tile_grass4; ctx.drawImage(img, i*rectSize, row*rectSize); break;
	}
        var type = data.rounds[round].rows[row][i];
        var img_tile = selectTile(round, type, row, i);
        ctx.drawImage(img_tile, i*rectSize, row*rectSize);
    }
}


function selectTile (round, type, row, col) {
    switch (type) {
    case 'X': //Rock
        return selectRock(round, type, row, col);
        break;
    case '.': //Empty
        return data.img.tile_floor;
        break;
    case 'C': //Capsule
        return data.img.tile_capsule;
        break;
    case 'B': //Ball
        return data.img.tile_ball;
        break;
    default:
        break;
    }
}


function selectRock (round, type, row, col) {
    var n = 0;
    var s = 0;
    var e = 0;
    var w = 0;

    if ((row-1) < 0) n = 0;
    else if (data.rounds[round].rows[row-1][col] == 'X') n = 1;
    if ((row+1) >= data.rows) s = 0;
    else if (data.rounds[round].rows[row+1][col] == 'X') s = 1;
    if ((col-1) < 0) e = 0;
    else if (data.rounds[round].rows[row][col-1] == 'X') e = 1;
    if ((col+1) >= data.cols) w = 0;
    else if (data.rounds[round].rows[row][col+1] == 'X') w = 1;

    if (n == 0 && s == 0 && w == 1 && e == 0) return data.img.tile_rock1;
    if (n == 0 && s == 0 && w == 1 && e == 1) return data.img.tile_rock2;
    if (n == 0 && s == 0 && w == 0 && e == 1) return data.img.tile_rock3;
    if (n == 1 && s == 0 && w == 0 && e == 0) return data.img.tile_rock4;
    if (n == 1 && s == 1 && w == 0 && e == 0) return data.img.tile_rock5;
    if (n == 0 && s == 1 && w == 0 && e == 0) return data.img.tile_rock6;
    if (n == 1 && s == 0 && w == 1 && e == 0) return data.img.tile_rock7;
    if (n == 1 && s == 0 && w == 0 && e == 1) return data.img.tile_rock8;
    if (n == 0 && s == 1 && w == 1 && e == 0) return data.img.tile_rock9;
    if (n == 0 && s == 1 && w == 0 && e == 1) return data.img.tile_rock10;
    if (n == 1 && s == 1 && w == 0 && e == 1) return data.img.tile_rock11;
    if (n == 1 && s == 0 && w == 1 && e == 1) return data.img.tile_rock12;
    if (n == 1 && s == 1 && w == 1 && e == 0) return data.img.tile_rock13;
    if (n == 0 && s == 1 && w == 1 && e == 1) return data.img.tile_rock14;
    if (n == 0 && s == 0 && w == 0 && e == 0) return data.img.tile_rock15;
    if (n == 1 && s == 1 && w == 1 && e == 1) return data.img.tile_rock16;
    return data.img.tile_rock;
}


// *********************************************************************
// Button events
// *********************************************************************

function playButton () {
    gamePaused = false;
}

function pauseButton () {
    gamePaused = true;
    gamePreview = true; //To call render again
    frames = 0;
}

function startButton () {
    actRound = 0;
    frames = 0;
    gamePreview = true;
    gamePaused = true;
}

function endButton () {
    actRound = data.nb_rounds;
    frames = 0;
    gamePreview = true;
}

function animButton () {
    gameAnim = !gameAnim;
}

function closeButton () {
    window.close();
}





// *********************************************************************
// Keyboard and Mouse events
// *********************************************************************

function onDocumentMouseWheel (event) {
}

function onDocumentKeyDown (event) {
}

function onDocumentKeyUp (event) {

    // http://www.webonweboff.com/tips/js/event_key_codes.aspx

    switch (event.keyCode) {

    case 36: // Start
        actRound = 0;
        frames = 0;
        gamePreview = true;
        break;

    case 35: // End
        actRound = data.nb_rounds;
        frames = 0;
        gamePreview = true;
        break;

    case 33: // PageDown
        actRound -= 10;
        frames = 0;
        gamePreview = true;
        break;

    case 34: // PageUp
        actRound += 10;
        frames = 0;
        gamePreview = true;
        break;

    case 38: // ArrowUp
    case 37: // ArrowLeft
        gamePaused= true;
        frames = 0;
        --actRound;
        gamePreview = true;
        break;

    case 40: // ArrowDown
    case 39: // ArrowRight
        gamePaused = true;
        frames = 0;
        ++actRound;
        gamePreview = true;
        break;

    case 32: // Space
        if (gamePaused) playButton();
        else pauseButton();
        break;

    case 72: // "h"
        help();
        break;

    default:
        //$("#debug").html(event.keyCode);
        break;
    }
}




function onWindowResize (event) {

    //Constants
    var header_height = 130;
    var canvas_margin = 20;

    // set canvas size
    var size = Math.min(document.body.offsetWidth, document.body.offsetHeight - header_height) - canvas_margin*2;

    canvas.width  = size;
    canvas.height = size;

    var max_dimension = Math.max(data.nb_cols, data.nb_rows);
    tileSize = size / max_dimension;

    drawGame();

}


function help () {
    // opens a new popup with the help page
    var win = window.open('help.html' , 'name', 'height=400, width=300');
    if (window.focus) win.focus();
    return false;
}



// *********************************************************************
// This function is called periodically.
// *********************************************************************

function mainloop () {

    // Configure buttons
    if (gamePaused) {
        $("#but_play").show();
        $("#but_pause").hide();
    } else {
        $("#but_play").hide();
        $("#but_pause").show();
    }

    if (actRound < 0) actRound = 0;
    if (actRound > data.nb_rounds) {
        actRound = data.nb_rounds;
        gamePaused = true;
        frames = 0;
    }

    if (!gamePaused || gamePreview) {

        updateGame();
        drawGame();
        writeGameState();

        if (gamePreview) {
            frames = 0;
            gamePreview = false;
        } else {
            frames++;
            if (frames == FRAMES_PER_ROUND) {
                actRound += gameDirection;
                frames = 0;
            }
        }

    }
    // periodically call mainloop
    var frame_time = 1000/speed;
    setTimeout(mainloop, frame_time);
}





// *********************************************************************
// Main function, it is called when the document is ready.
// *********************************************************************

function init () {

    // get url parameters
    var game;
    if (getURLParameter("sub") != null) {
        var domain = window.location.protocol + "//" + window.location.host;
        if (getURLParameter("nbr") != null) {
            game = domain + "/?cmd=lliuraments&sub="+getURLParameter("sub")+"&nbr="+getURLParameter("nbr")+"&download=partida";
        } else {
            game = domain + "/?cmd=partida&sub="+getURLParameter("sub")+"&download=partida";
        }
    } else {
        game = getURLParameter("game");
    }

    if (game == null || game == "") {
        // ask the user for a game input
        var inputdiv = document.getElementById('inputdiv')
        inputdiv.style.display = "";
        document.getElementById('file').addEventListener('change', function(evt) {
            //http://www.html5rocks.com/en/tutorials/file/dndfiles/
            var file = evt.target.files[0];
            var reader = new FileReader();
            reader.readAsText(file);
            reader.onloadend = function(evt) {
                if (evt.target.readyState == FileReader.DONE) { // DONE == 2
                    inputdiv.style.display = "none";
                    document.getElementById("loadingdiv").style.display="";
                    initGame(reader.result);
                } else {
                    alert("Error accessing file");
                }
            };
        }, false);
    } else {
        document.getElementById("loadingdiv").style.display="";
        // load the given game
        loadFile(game, initGame);
    }

}
