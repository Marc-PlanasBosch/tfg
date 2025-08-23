// *********************************************************************
// Global variables
// *********************************************************************

var randomNumber = 1;

// Viewer state
var gamePaused = true;
var gamePreview = false; //If true, render will be called for the next tick even if game is paused, and then will be set to false
var gameDirection = 1;
var gameAnim = true;
var gameAudio = true;
var actRound = 0; //Current round index

// Data
var raw_data_str; //String for storing the raw data
var dataLoaded = false; //Set to true when raw_data_str is ready to be parsed
var data = { } //Object for storing all the game data

// Animation
var speed = 250; //Ticks per second
var FRAMES_PER_ROUND = 8;
var frames = 0; //Incremented each tick, when it reaches FRAMES_PER_ROUND, actRound is updated (acording to gameDirection)

// Visuals
var unitSize = 0.6; // 1 = same size as tile
var unitLineWidth = 2;
var grid_color = "#888888";
var player_colors = {
		'0': "0D792B", // green
	    '1': "DC4328", // orange-red
	    '2': "59C6F2", // cyan
	    '3': "FFED00", // yellow
}
var flip_squirtle = false;
var flip_charmander = false;
var flip_bulbasaur = false;

// *********************************************************************
// Sounds
// *********************************************************************

var snds = new Object();
snds.song		= new Audio("snd/ambient.ogg");
snds.attack     = new Audio("snd/attackup.ogg");
snds.defense    = new Audio("snd/defenseup.ogg");
snds.scope    	= new Audio("snd/scopeup.ogg");
snds.stone      = new Audio("snd/stone.ogg");
snds.pointbag   = new Audio("snd/pointsbag.ogg");
snds.player0 	= new Audio("snd/bulbasaur.ogg");
snds.player1	= new Audio("snd/charmander.ogg");
snds.player2	= new Audio("snd/squirtle.ogg");
snds.player3	= new Audio("snd/pikachu.ogg");
snds.victory	= new Audio("snd/victory.ogg");

snds.fx_on = (snds.song == null);

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

    s = t[p++]; // "seed"
    s = t[p++]; // number of seed
    
    s = t[p++];
    data.version = t[p++];
    if (s != "Poquemon") alert("Error at round " + round + ".\n" + "Poquémon");
    if (data.version != "v1") alert("Error at round " + round + ".\n" + "version");

    s = t[p++];
    data.nb_players = int(t[p++]);
    if (s != "nb_players") alert("Error at round " + round + ".\n" + "nb_players");

    s = t[p++];
    data.nb_poquemon = int(t[p++]);
    if (s != "nb_poquemon") alert ("Error at round " + round + ".\n" + "nb_poquemon");

    s = t[p++];
    data.nb_rounds = int(t[p++]);
    if (s != "nb_rounds") alert("Error at round " + round + ".\n" + "nb_rounds");

    s = t[p++];
    data.nb_ghost_wall = int(t[p++]);
    if (s != "nb_ghost_wall") alert("Error at round " + round + ".\n" + "nb_ghost_wall");

    s = t[p++];
    data.nb_points_bag = int(t[p++]);
    if (s != "nb_point") alert("Error at round " + round + ".\n" + "nb_point");

    s = t[p++];
    data.nb_stone = int(t[p++]);
    if (s != "nb_stone") alert("Error at round " + round + ".\n" + "nb_stone");

    s = t[p++];
    data.nb_scope = int(t[p++]);
    if (s != "nb_scope") alert("Error at round " + round + ".\n" + "nb_scope");

    s = t[p++];
    data.nb_attack = int(t[p++]);
    if (s != "nb_attack") alert("Error at round " + round + ".\n" + "nb_attack");

    s = t[p++];
    data.nb_defense = int(t[p++]);
    if (s != "nb_defense") alert("Error at round " + round + ".\n" + "nb_defense");

    s = t[p++];
    data.player_regen_time = int(t[p++]);
    if (s != "player_regen_time") alert("Error at round " + round + ".\n" + "player_regen_time");

    s = t[p++];
    data.wall_change_time = int(t[p++]);
    if (s != "wall_change_time") alert("Error at round " + round + ".\n" + "wall_change_time");

    s = t[p++];
    data.bag_regen_time = int(t[p++]);
    if (s != "point_regen_time") alert("Error at round " + round + ".\n" + "point_regen_time");

    s = t[p++];
    data.stone_regen_time = int(t[p++]);
    if (s != "stone_regen_time") alert("Error at round " + round + ".\n" + "stone_regen_time");

    s = t[p++];
    data.scope_regen_time = int(t[p++]);
    if (s != "scope_regen_time") alert("Error at round " + round + ".\n" + "scope_regen_time");

    s = t[p++];
    data.attack_regen_time = int(t[p++]);
    if (s != "attack_regen_time") alert("Error at round " + round + ".\n" + "attack_regen_time");

    s = t[p++];
    data.defense_regen_time = int(t[p++]);
    if (s != "defense_regen_time") alert("Error at round " + round + ".\n" + "defense_regen_time");

    s = t[p++];
    data.battle_reward = int(t[p++]);
    if (s != "battle_reward") alert("Error at round " + round + ".\n" + "battle_reward");

    s = t[p++];
    data.max_scope = int(t[p++]);
    if (s != "max_scope") alert("Error at round " + round + ".\n" + "max_scope");

    s = t[p++];
    data.max_stone = int(t[p++]);
    if (s != "max_stone") alert("Error at round " + round + ".\n" + "max_stone");

    s = t[p++];
    data.rows = int(t[p++]);
    if (s != "rows") alert("Error at round " + round + ".\n" + "rows");

    s = t[p++];
    data.cols = int(t[p++]);
    if (s != "cols") alert("Error at round " + round + ".\n" + "cols");

    // reading player names
    s = t[p++];
    if (s != "names") alert("Error at round " + round + ".\n" + "names");
    data.names = new Array();
    for (var i = 0; i < data.nb_players; ++i) {
        data.names[i] = t[p++];
    }

    // reading round info
    data.rounds = new Array();
    for (var round = 0; round <= data.nb_rounds; ++round) {

        $("#debug").html(round);
        
        // round #round
        if (t[p++] != "round") alert("Error at round " + round + ".\n" + "round");
        if (int(t[p++]) != round) alert("Error at round " + round + ".\n" + "wrong round");

        // maze
        if (t[p++] != "board") alert ("Error at round " + round + ".\n" + "board");
        t[p++]; // 00000000001111111111
        t[p++]; // 01234567890123456789
        
        data.rounds[round] = new Object();
        data.rounds[round].rows = new Array();
        for (var i = 0; i < data.rows; ++i) {
            // ignore num index row
            t[p++];
            data.rounds[round].rows[i] = t[p++];
        }

        // status
        if (t[p++] != "status") alert("Error at round " + round + ".\n" + "status");
        data.rounds[round].cpu  = new Array();
        for (var i = 0; i < data.nb_players; ++i) {
            data.rounds[round].cpu[i] = double(t[p++]);
        }
        
        // Poquémon
        if (t[p++] != "poquemon") alert("Error at round " + round + ".\n" + "poquemon");
        // ignore header
        t[p++]; t[p++]; t[p++]; t[p++]; t[p++]; 
        t[p++]; t[p++]; t[p++]; t[p++]; t[p++]; t[p++];
        
        data.rounds[round].team = new Array();
        for (var i = 0; i < data.nb_players; ++i) {
            data.rounds[round].team[i] = new Object();
            data.rounds[round].team[i].id       =   int(t[p++]);
            data.rounds[round].team[i].player   =   int(t[p++]); 
            data.rounds[round].team[i].i        =   int(t[p++]);
            data.rounds[round].team[i].j        =   int(t[p++]);
            data.rounds[round].team[i].attack   =   int(t[p++]);
            data.rounds[round].team[i].defense  =   int(t[p++]);
            data.rounds[round].team[i].scope    =   int(t[p++]);
            data.rounds[round].team[i].stone    =   int(t[p++]);
            data.rounds[round].team[i].points   =   int(t[p++]);
            data.rounds[round].team[i].time     =   int(t[p++]);
            data.rounds[round].team[i].alive    =   	t[p++];
        }
        
        // wall ghosts
        if (t[p++] != "walls") alert("Error at round " + round + ".\n" + "walls");
        // ignore header 
        t[p++]; t[p++]; t[p++]; t[p++];
        
        data.rounds[round].walls = new Array();
        for (var k = 0; k < data.nb_ghost_wall; ++k) {
            data.rounds[round].walls[k] = new Object();
            data.rounds[round].walls[k].i       = int(t[p++]);
            data.rounds[round].walls[k].j       = int(t[p++]);
            data.rounds[round].walls[k].time    = int(t[p++]);
            data.rounds[round].walls[k].present = t[p++] == 1; 
        }
        
        // bonus
        if (t[p++] != "bonus") alert("Error at round " + round + ".\n" + "bonus");
        // ignore header
        t[p++]; t[p++]; t[p++]; t[p++]; t[p++]; t[p++];
        
        data.rounds[round].points_bag = new Array();
        for (var k = 0; k < data.nb_points_bag; ++k) {
            data.rounds[round].points_bag[k] = new Object();
            data.rounds[round].points_bag[k].type = t[p++];
            if (data.rounds[round].points_bag[k].type != 'P') 
                alert("Error at round " + round + ".\n" + "type bonus is not P");
            data.rounds[round].points_bag[k].i = int(t[p++]);
            data.rounds[round].points_bag[k].j = int(t[p++]); 
            data.rounds[round].points_bag[k].pts = int(t[p++]);
            data.rounds[round].points_bag[k].time = int(t[p++]);
            data.rounds[round].points_bag[k].present = t[p++] == 1;
        }
        
        data.rounds[round].stone = new Array();
        for (var k = 0; k < data.nb_stone; ++k) {
            data.rounds[round].stone[k] = new Object();
            data.rounds[round].stone[k].type = t[p++];
            if (data.rounds[round].stone[k].type != 'S') 
                alert("Error at round " + round + ".\n" + "type bonus is not S");
            data.rounds[round].stone[k].i = int(t[p++]);
            data.rounds[round].stone[k].j = int(t[p++]); 
            data.rounds[round].stone[k].pts = int(t[p++]);
            data.rounds[round].stone[k].time = int(t[p++]);
            data.rounds[round].stone[k].present = t[p++] == 1;
        }
        
        data.rounds[round].scope = new Array();
        for (var k = 0; k < data.nb_scope; ++k) {
            data.rounds[round].scope[k] = new Object();
            data.rounds[round].scope[k].type = t[p++];
            if (data.rounds[round].scope[k].type != 'R') 
                alert("Error at round " + round + ".\n" + "type bonus is not R");
            data.rounds[round].scope[k].i = int(t[p++]);
            data.rounds[round].scope[k].j = int(t[p++]); 
            data.rounds[round].scope[k].pts = int(t[p++]);
            data.rounds[round].scope[k].time = int(t[p++]);
            data.rounds[round].scope[k].present = t[p++] == 1;
        }
        
        data.rounds[round].attack = new Array();
        for (var k = 0; k < data.nb_attack; ++k) {
            data.rounds[round].attack[k] = new Object();
            data.rounds[round].attack[k].type = t[p++];
            if (data.rounds[round].attack[k].type != 'A') 
                alert("Error at round " + round + ".\n" + "type bonus is not A");
            data.rounds[round].attack[k].i = int(t[p++]);
            data.rounds[round].attack[k].j = int(t[p++]); 
            data.rounds[round].attack[k].pts = int(t[p++]);
            data.rounds[round].attack[k].time = int(t[p++]);
            data.rounds[round].attack[k].present = t[p++] == 1;
        }
        
        data.rounds[round].defense = new Array();
        for (var k = 0; k < data.nb_defense; ++k) {
            data.rounds[round].defense[k] = new Object();
            data.rounds[round].defense[k].type = t[p++];
            if (data.rounds[round].defense[k].type != 'D') 
                alert("Error at round " + round + ".\n" + "type bonus is not D");
            data.rounds[round].defense[k].i = int(t[p++]);
            data.rounds[round].defense[k].j = int(t[p++]); 
            data.rounds[round].defense[k].pts = int(t[p++]);
            data.rounds[round].defense[k].time = int(t[p++]);
            data.rounds[round].defense[k].present = t[p++] == 1;
        }
        
        if (round != data.nb_rounds) {
            // actions asked
            if (t[p++] != "actions_asked") 
                alert("Error at round " + round + ".\n" + "no actions_asked");
            // ignore header
            t[p++]; t[p++]; t[p++];
            for (var i = 0; i < data.nb_players; ++i) {
                if (int(t[p++]) != i) 
                    alert("Error at round " + round + ".\n" + "wrong player");
		        // ignore
                t[p++];
                t[p++];
            }

            // actions done
            if (t[p++] != "actions_done") 
                alert("Error at round " + round + ".\n" + "no actions_done");
            // ignore header
            t[p++]; t[p++]; t[p++];
            for (var i = 0; i < data.nb_players; ++i) {
                if (int(t[p++]) != i) 
                    alert("Error at round " + round + ".\n" + "wrong player");
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

    // floor
    data.img.tile_floor0 = new Image();
    data.img.tile_floor0.src = "img/tile_floor00.png";
    data.img.tile_floor1 = new Image();
    data.img.tile_floor1.src = "img/tile_floor01.png";

    // floor
    data.img.tile_floor = new Image();
    data.img.tile_floor.src = "img/tile_floor.png";

    // rocks
    data.img.tile_rock = new Image();
    data.img.tile_rock.src = "img/tile_rock.png";
    data.img.tile_rock1 = new Image();
    data.img.tile_rock1.src = "img/tile_rock01.png";
    data.img.tile_rock2 = new Image();
    data.img.tile_rock2.src = "img/tile_rock02.png";
    data.img.tile_rock3 = new Image();
    data.img.tile_rock3.src = "img/tile_rock03.png";
    data.img.tile_rock4 = new Image();
    data.img.tile_rock4.src = "img/tile_rock04.png";
    data.img.tile_rock5 = new Image();
    data.img.tile_rock5.src = "img/tile_rock05.png";
    data.img.tile_rock6 = new Image();
    data.img.tile_rock6.src = "img/tile_rock06.png";
    data.img.tile_rock7 = new Image();
    data.img.tile_rock7.src = "img/tile_rock07.png";
    data.img.tile_rock8 = new Image();
    data.img.tile_rock8.src = "img/tile_rock08.png";
    data.img.tile_rock9 = new Image();
    data.img.tile_rock9.src = "img/tile_rock09.png";
    data.img.tile_rock10 = new Image();
    data.img.tile_rock10.src = "img/tile_rock010.png";
    data.img.tile_rock11 = new Image();
    data.img.tile_rock11.src = "img/tile_rock011.png";
    data.img.tile_rock12 = new Image();
    data.img.tile_rock12.src = "img/tile_rock012.png";
    data.img.tile_rock13 = new Image();
    data.img.tile_rock13.src = "img/tile_rock013.png";
    data.img.tile_rock14 = new Image();
    data.img.tile_rock14.src = "img/tile_rock014.png";
    data.img.tile_rock15 = new Image();
    data.img.tile_rock15.src = "img/tile_rock015.png";
    data.img.tile_rock16 = new Image();
    data.img.tile_rock16.src = "img/tile_rock016.png";

    // bonus
    data.img.tile_point_bag100 = new Image();
    data.img.tile_point_bag100.src = "img/point_bag100.png";
    data.img.tile_point_bag200 = new Image();
    data.img.tile_point_bag200.src = "img/point_bag200.png";
    data.img.tile_point_bag300 = new Image();
    data.img.tile_point_bag300.src = "img/point_bag300.png";
    data.img.tile_point_bag400 = new Image();
    data.img.tile_point_bag400.src = "img/point_bag400.png";
    data.img.tile_point_bag500 = new Image();
    data.img.tile_point_bag500.src = "img/point_bag500.png";
    data.img.tile_stone = new Image();
    data.img.tile_stone.src = "img/stone.png";
    data.img.tile_scope = new Image();
    data.img.tile_scope.src = "img/scope.png";
    data.img.tile_attack = new Image();
    data.img.tile_attack.src = "img/attack.png";
    data.img.tile_defense = new Image();
    data.img.tile_defense.src = "img/defense.png";
    
    // Poquémon
    data.img.spr_poquemon = new Array();
    for (var i = 0; i < data.nb_players; ++i) {
        data.img.spr_poquemon[i] = new Image();
        data.img.spr_poquemon[i].src = "img/player" + i + ".png";
    }
    
    data.img.flip_squirtle = new Image();
    data.img.flip_squirtle.src = "img/flip_squirtle.png";
    
    data.img.flip_charmander = new Image();
    data.img.flip_charmander.src = "img/flip_charmander.png";
    
    data.img.flip_bulbasaur = new Image();
    data.img.flip_bulbasaur.src = "img/flip_bulbasaur.png";
    
    
    // attack_poquemon
    data.img.spr_attack = new Array()
    for (var i = 0; i < data.nb_players; ++i) {
        data.img.spr_attack[i] = new Array();
        data.img.spr_attack[i].ini_b2t = new Image();
        data.img.spr_attack[i].ini_b2t.src = "img/at" + i + "-ini-b2t.png";
        data.img.spr_attack[i].ini_t2b = new Image();
        data.img.spr_attack[i].ini_t2b.src = "img/at" + i + "-ini-t2b.png";
        data.img.spr_attack[i].ini_l2r = new Image();
        data.img.spr_attack[i].ini_l2r.src = "img/at" + i + "-ini-l2r.png";
        data.img.spr_attack[i].ini_r2l = new Image();
        data.img.spr_attack[i].ini_r2l.src = "img/at" + i + "-ini-r2l.png";

        data.img.spr_attack[i].fin_b2t = new Image();
        data.img.spr_attack[i].fin_b2t.src = "img/at" + i + "-fin-b2t.png";
        data.img.spr_attack[i].fin_t2b = new Image();
        data.img.spr_attack[i].fin_t2b.src = "img/at" + i + "-fin-t2b.png";
        data.img.spr_attack[i].fin_l2r = new Image();
        data.img.spr_attack[i].fin_l2r.src = "img/at" + i + "-fin-l2r.png";
        data.img.spr_attack[i].fin_r2l = new Image();
        data.img.spr_attack[i].fin_r2l.src = "img/at" + i + "-fin-r2l.png";

        data.img.spr_attack[i].v = new Image();
        data.img.spr_attack[i].v.src = "img/at" + i + "-v.png";
        data.img.spr_attack[i].h = new Image();
        data.img.spr_attack[i].h.src = "img/at" + i + "-h.png";
    }
}


// *********************************************************************
// Main loop functions
// *********************************************************************

function updateGame () {
    $("#slider").slider("option", "value", actRound); 
}


function writeGameState (player) {
    // write round
    $("#round").html("Round: " + actRound);

    //update scoreboard
    score = document.getElementById("scores" + player);
    
    var height = $(window).height() * (2/5); 
    $("#scores" + player).height(height);
    score.style.paddingTop = height/4;
    
    var scoreboard = "";

    scoreboard += "<td style='background-color:#000000;'>";

    scoreboard += "<span class='score" + player + "'>"
        + "<div style='display:inline-block; width:30px; height:30px; '><img src =" +  "'img/player" + player + ".png'" + " height='30' width='30'></div>"
        + "<div style='display:inline-block; vertical-align: middle; margin-bottom: 7px; margin-left:8px; font-size: 10pt;'>"+data.names[player]+"</div>"
        + "<br/>"
        + "<div style='margin-left: 10px;'>"
        + "<div style='padding:2px; font-size: 10pt;'> Score: " + data.rounds[actRound].team[player].points + "</div>"
        + "<div style='padding:2px; font-size: 10pt; vertical-align:middle;'><img src ='img/attack.png' height='20' width='20'> Attack: " + data.rounds[actRound].team[player].attack + "</div>"
        + "<div style='padding:2px; font-size: 10pt; vertical-align:middle;'><img src ='img/defense.png' height='20' width='20'> Defense: " + data.rounds[actRound].team[player].defense + "</div>"
        + "<div style='padding:2px; font-size: 10pt; vertical-align:middle;'><img src ='img/scope.png' height='20' width='20'> Scope: " + data.rounds[actRound].team[player].scope + "</div>"
        + (data.secgame? "<div style='font-size: 10pt; padding:2px; margin-top: 5px;'> CPU: " + (data.rounds[actRound].cpu[player] == -1 ? " <font color=\"red\"><b>OUT</b></font>" : int(1000*data.rounds[actRound].cpu[player])/10. + "%") + "</div>" : "")
        + "</div>"
        + "</span>";
    

    $("#scores" + player).html(scoreboard);
}

function drawGame () {

	if (!snds.victory.paused) snds.victory.pause();
	
    if (snds.song != null && gameAudio && !gamePaused) {
    	snds.song.addEventListener('ended', function() {
    	    this.currentTime = 0;
    	    this.play();
    	}, false);
    	snds.song.volume = 0.5;
     	snds.song.play();
    }
    else 
    	snds.song.pause();
    
    if (data.nb_rounds - actRound < 5) snds.song.volume = snds.song.volume * 0.5;
	
    if (actRound < data.nb_rounds && !snds.fx_on) {
		if (frames == 1) {
		    if (actRound + 1 <= data.nb_rounds) {
				for (var i = 0; i < data.nb_players; i++) {
				    if (data.rounds[actRound + 1].team[i].action_type == 'a') {
				    	switch(i) {
					    	case 0: 
					    		snds.player0.play();
					    		break;
					    	case 1:
					    		snds.player1.play();
					    		break;
					    	case 2:
					    		snds.player2.play();
					    		break;
					    	case 3:
					    		snds.player3.play();
					    		break;
				    	}
				    }
				}
		    }
		    
		    var one_found = false;
		    for (var k = 0; k < data.nb_points_bag && !one_found; ++k)
			    if (data.rounds[actRound].points_bag[k].present == 1 && data.rounds[actRound+1].points_bag[k].present != 1) {
			    	snds.pointbag.play();
			    	one_found = true;
			    }
		    
		    one_found = false;
		    for (var k = 0; k < data.nb_stone && !one_found; ++k)
			    if (data.rounds[actRound].stone[k].present == 1 && data.rounds[actRound+1].stone[k].present != 1) {
			    	snds.stone.play();
			    	one_found = true;
			    }
		    
		    one_found = false;
		    for (var k = 0; k < data.nb_scope && !one_found; ++k)
			    if (data.rounds[actRound].scope[k].present == 1 && data.rounds[actRound+1].scope[k].present != 1) {
			    	snds.scope.play();
			    	one_found = true;
			    }
		    
		    one_found = false;
		    for (var k = 0; k < data.nb_attack && !one_found; ++k)
			    if (data.rounds[actRound].attack[k].present == 1 && data.rounds[actRound+1].attack[k].present != 1) {
			    	snds.attack.play();
			    	one_found = true;
			    }
		    
		    one_found = false;
		    for (var k = 0; k < data.nb_defense && !one_found; ++k)
			    if (data.rounds[actRound].defense[k].present == 1 && data.rounds[actRound+1].defense[k].present != 1) {
			    	snds.defense.play();
			    	one_found = true;
			    }
		}
		else if (frames == 3) {
	
		    /*for (var i = 0; i < data.nb_players; i++) {
				if (data.rounds[actRound].team[i].state == 'a' && data.rounds[actRound+1].team[i].state == 'd') {
				    snds.death.play();
				}
				if (actRound + 6 <= data.nb_rounds) {
				    if (data.rounds[actRound+5].team[i].state == 'd' && data.rounds[actRound+6].team[i].state == 'a') {
				    	snds.regeneration.play();
				    }
				}
		    }*/
		}
    }
    
    else if (actRound == data.nb_rounds){
    	snds.song.pause();
    	snds.victory.play();
    }
    
    if (canvas.getContext) {
        var context = canvas.getContext('2d');
        var rectSize = data.tileSize;

		var fixedMargin = 90;
		if (double(data.rows)/double(data.cols) > 0.9) {
		    fixedMargin = 10;
		}
		var heightScore = 150;
	
		canvas.width  = window.innerWidth  - fixedMargin - heightScore;
	    canvas.height = window.innerHeight - fixedMargin - heightScore;
	
		var sw = canvas.width  /(rectSize*data.cols);
		var sh = canvas.height/(rectSize*data.rows);
		var s;
		if (sw < sh) {
		    s = sw;
		    var offset = (canvas.height - s*rectSize*data.rows)/ 2;
		    canvas.style.marginTop  = fixedMargin + offset;
		    canvas.style.marginLeft = fixedMargin;
		    canvas.style.marginRight = fixedMargin;
		}
		else {
		    s = sh;
		    var offset = (canvas.width - s*rectSize*data.cols)/ 2;
		    canvas.style.marginTop  = fixedMargin;
		    canvas.style.marginLeft = fixedMargin + offset;
		    //canvas.style.marginRight = fixedMargin + offset;
		}
	    context.scale(s, s);
	
	    // outer rectangle
	    context.fillStyle = "rgb(0,0,0)";
	    context.fillRect(0, 0, rectSize*data.cols, rectSize*data.rows);
	
		randomNumber = 1;
	    // draw maze
	    for (var i = 0; i < data.rows; i++)
	        drawRow(actRound, i);

		// draw bonus
	    for (var k = 0; k < data.nb_points_bag; ++k){
		    if (data.rounds[actRound].points_bag[k].present) {
	                var x = data.rounds[actRound].points_bag[k].i;
	                var y = data.rounds[actRound].points_bag[k].j;
	                var img;
	                switch(data.rounds[actRound].points_bag[k].pts) {
	                	case 100:
	                		img = data.img.tile_point_bag100;
	                		break;
	                	case 200:
	                		img = data.img.tile_point_bag200;
	                		break;
	                	case 300:
	                		img = data.img.tile_point_bag300;
	                		break;
	                	case 400:
	                		img = data.img.tile_point_bag400;
	                		break;
	                	case 500:
	                		img = data.img.tile_point_bag500;
	                		break;
	                }
	                var ctx = canvas.getContext('2d');
	                ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
		    }
	    }
	    for (var k = 0; k < data.nb_stone; ++k)
		    if (data.rounds[actRound].stone[k].present) {
	                var x = data.rounds[actRound].stone[k].i;
	                var y = data.rounds[actRound].stone[k].j;
	                var img = data.img.tile_stone;
	                var ctx = canvas.getContext('2d');
	                ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
		    }
	    for (var k = 0; k < data.nb_scope; ++k)
		    if (data.rounds[actRound].scope[k].present) {
	                var x = data.rounds[actRound].scope[k].i;
	                var y = data.rounds[actRound].scope[k].j;
	                var img = data.img.tile_scope;
	                var ctx = canvas.getContext('2d');
	                ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
		    }
	    for (var k = 0; k < data.nb_attack; ++k)
		    if (data.rounds[actRound].attack[k].present) {
	                var x = data.rounds[actRound].attack[k].i;
	                var y = data.rounds[actRound].attack[k].j;
	                var img = data.img.tile_attack;
	                var ctx = canvas.getContext('2d');
	                ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
		    }
	    for (var k = 0; k < data.nb_defense; ++k)
		    if (data.rounds[actRound].defense[k].present) {
	                var x = data.rounds[actRound].defense[k].i;
	                var y = data.rounds[actRound].defense[k].j;
	                var img = data.img.tile_defense;
	                var ctx = canvas.getContext('2d');
	                ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
		    }
	
	    // draw Poquémons
		var attack_i = -1;
		var attack_j = -1;
		var attack_dir = 'n';
		var id_player = -1;
		var scope_player = -1;


	    for (var i = 0; i < data.nb_players; i++) {
	
	       	if (data.rounds[actRound].team[i].alive == 'a') {
	
	            if (! gameAnim) {
	                var x = data.rounds[actRound].team[i].i;
	                var y = data.rounds[actRound].team[i].j;
	                var ctx = canvas.getContext('2d');
					var img = data.img.spr_poquemon[i];
					ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
	            } 
	            else {
		    		if (data.rounds[actRound].team[i].action_type == 'a') {
						attack_i = data.rounds[actRound].team[i].i;
						attack_j = data.rounds[actRound].team[i].j;
						attack_dir   = data.rounds[actRound].team[i].action_dir;
						id_player = data.rounds[actRound].team[i].player;
						scope_player = data.rounds[actRound].team[i].scope;
				    }
				    
					var x = data.rounds[actRound].team[i].i;
					x = x*data.tileSize;					
					if (data.rounds[actRound].team[i].action_type !='a')
					    x = setMovedX(x, data.rounds[actRound].team[i].action_dir);
	
					var y = data.rounds[actRound].team[i].j;
					y = y*data.tileSize;
					if (data.rounds[actRound].team[i].action_type !='a')
					    y = setMovedY(y, data.rounds[actRound].team[i].action_dir);
		            var ctx = canvas.getContext('2d');
	
			    	// regenerated Poquémon are surrounded by a big thin circle
		            if (actRound > 6 && data.rounds[actRound - 6].team[i].alive == 'd') {
						ctx.beginPath();
						ctx.strokeStyle = "#" + player_colors[i];
		                ctx.lineWidth = 2;
		                ctx.arc(y+25, x+25, 64, 0, Math.PI*2, true);
		                ctx.closePath();
		                ctx.stroke();
		            }
	
					var img = data.img.spr_poquemon[i];
					
					if (i == 0 && actRound < data.nb_rounds) {
						var nextY = setMovedY(y, data.rounds[actRound+1].team[i].action_dir);
						if (y < nextY) {
							flip_bulbasaur = true;
							img = data.img.flip_bulbasaur;
						}
						else if ( y > nextY)
						{
							flip_bulbasaur = false;
							img = data.img.spr_poquemon[i];
						}
						else {
							img = flip_bulbasaur ? data.img.flip_bulbasaur : data.img.spr_poquemon[i];
						}
					}
					else if (i == 1 && actRound < data.nb_rounds) {
						var nextY = setMovedY(y, data.rounds[actRound+1].team[i].action_dir);
						if (y < nextY) {
							flip_charmander = true;
							img = data.img.flip_charmander;
						}
						else if ( y > nextY)
						{
							flip_charmander = false;
							img = data.img.spr_poquemon[i];
						}
						else {
							img = flip_charmander ? data.img.flip_charmander : data.img.spr_poquemon[i];
						}
					}			
					else if (i == 2 && actRound < data.nb_rounds) {
						var nextY = setMovedY(y, data.rounds[actRound+1].team[i].action_dir);
						if (y > nextY) {
							flip_squirtle = true;
							img = data.img.flip_squirtle;
						}
						else if (y < nextY)
						{;
							flip_squirtle = false;
							img = data.img.spr_poquemon[i];
						}
						else {
							img = flip_squirtle ? data.img.flip_squirtle : data.img.spr_poquemon[i];
						}
					}
 
					
					ctx.drawImage(img, y, x);
		       }
		    } 
	    	else {
	
				// regenerated Pokémon are surrounded by a big thin circle
	            if (actRound <= data.nb_rounds - 6 && data.rounds[actRound + 6].team[i].alive == 'a') {
	
	                var r = 6;
	                while (data.rounds[actRound + r].team[i].alive != 'd') r--;
	                r++;
	
	                var x = data.rounds[actRound + r].team[i].i;
	                x = x*data.tileSize;
	
	                var y = data.rounds[actRound + r].team[i].j;
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
	
		// draw attacks
	   	if (frames < (FRAMES_PER_ROUND/2))
	    	drawAttack(attack_i, attack_j, attack_dir, id_player, scope_player);
	}
}

function drawAttack(i, j, dir, id_player, scope_player) {

    if (dir == 'n') return;

    var x = i;
    var y = j;

    // do not show attacks that are degenerate (== there is no room)
    if (data.rounds[actRound].rows[x][y] == 'X') return;

    var ctx = canvas.getContext('2d');
    
    // paint initial part of attack
    if      (dir == 't') img = data.img.spr_attack[id_player].ini_b2t;
    else if (dir == 'b') img = data.img.spr_attack[id_player].ini_t2b;
    else if (dir == 'l') img = data.img.spr_attack[id_player].ini_r2l;
    else if (dir == 'r') img = data.img.spr_attack[id_player].ini_l2r;
    ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
    
    if      (dir == 't') --x;
    else if (dir == 'b') ++x;
    else if (dir == 'l') --y;
    else if (dir == 'r') ++y; 
    	
	var dist = 1;
	
	// Boolean to control the number of intermediate images. We will left 1 for the end
	var endAttack = dist == scope_player || playerFound(x,y, actRound);
	
	
	// using that the board is surrounded by rocks
	
    while (data.rounds[actRound].rows[x][y] != 'X' &&  !endAttack) {
    	
		// paint middle part of attacks
		if (dir == 't' || dir == 'b') img = data.img.spr_attack[id_player].v;
		else                          img = data.img.spr_attack[id_player].h;
		ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
	
		if      (dir == 't') --x;
		else if (dir == 'b') ++x;
		else if (dir == 'l') --y;
		else if (dir == 'r') ++y;
		
		++dist; 
		
		if (dist == scope_player || playerFound(x,y, actRound)) 
    		endAttack = true;   	
		
    }

    // paint final part of attacks
    if      (dir == 't') img = data.img.spr_attack[id_player].fin_b2t;
    else if (dir == 'b') img = data.img.spr_attack[id_player].fin_t2b;
    else if (dir == 'l') img = data.img.spr_attack[id_player].fin_r2l;
    else if (dir == 'r') img = data.img.spr_attack[id_player].fin_l2r;
    ctx.drawImage(img, y*data.tileSize, x*data.tileSize);
}

function playerFound(x, y, actRound) {
	for(var i = 0; i < data.nb_players; ++i) {
		if (data.rounds[actRound].team[i].i == x &&
				data.rounds[actRound].team[i].j == y &&
				data.rounds[actRound].team[i].alive == 'a') return true;
	}
	return false;
}


function setMovedX (x, action) {
    if (action == 'n') return x;
    if (action == 't') {
        return (x - ((frames*data.tileSize)/FRAMES_PER_ROUND));
    }
    if (action == 'b') {
        return (x + ((frames*data.tileSize)/FRAMES_PER_ROUND));
    }
    return x;
}

function setMovedY (y, action) {
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
		switch (randomNumber % 2) {
			case 0: var img = data.img.tile_floor0; ctx.drawImage(img, i*rectSize, row*rectSize); break;
			case 1: var img = data.img.tile_floor1; ctx.drawImage(img, i*rectSize, row*rectSize); break;
		}
	        var type = data.rounds[round].rows[row][i];
	        var img_tile = selectTile(round, type, row, i);
	        ctx.drawImage(img_tile, i*rectSize, row*rectSize);
    }
}

function selectTile (round, type, row, col) {
    switch (type) {
    case 'X': //Wall
        return selectRock(round, type, row, col);
        break;
    case '.': //Empty
        return data.img.tile_floor;
        break;
    case 'P': //Points_bag
    	for (var k = 0; k < data.nb_points_bag; ++k){
		    if (row == data.rounds[round].points_bag[k].i &&
		    	col == data.rounds[round].points_bag[k].j &&
		    	data.rounds[round].points_bag[k].present) {
		    		
	                switch(data.rounds[round].points_bag[k].pts) {
	                	case 100:
	                		return data.img.tile_point_bag100;
	                		break;
	                	case 200:
	                		return data.img.tile_point_bag200;
	                		break;
	                	case 300:
	                		return data.img.tile_point_bag300;
	                		break;
	                	case 400:
	                		return data.img.tile_point_bag400;
	                		break;
	                	case 500:
	                		return data.img.tile_point_bag500;
	                		break;
	                }
		    }
	    }
        return data.img.tile_point_bag;
        break;
    case 'S': //Stone
        return data.img.tile_stone;
        break;
    case 'R': //Scope
        return data.img.tile_scope;
        break;
    case 'A': //Attack_up
        return data.img.tile_attack;
        break;
    case 'D': //Defense_up
        return data.img.tile_defense;
        break;
    case 'M': //Ghost_Wall
    	for (var k = 0; k < data.nb_ghost_wall; ++k) {
    		if (data.rounds[round].walls[k].i == row
    				&& data.rounds[round].walls[k].j == col
    				&& data.rounds[round].walls[k].present)
    			return selectRock(round, type, row, col)
    	}
    	return data.img.tile_floor;
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
    else if (data.rounds[round].rows[row-1][col] == 'X' || isGhostWall(round, row-1, col)) n = 1;
    if ((row+1) >= data.rows) s = 0;
    else if (data.rounds[round].rows[row+1][col] == 'X' || isGhostWall(round, row+1, col)) s = 1;
    if ((col-1) < 0) e = 0;
    else if (data.rounds[round].rows[row][col-1] == 'X' || isGhostWall(round, row, col-1)) e = 1;
    if ((col+1) >= data.cols) w = 0;
    else if (data.rounds[round].rows[row][col+1] == 'X' || isGhostWall(round, row, col+1)) w = 1;
    
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

function isGhostWall(round, row, col) {
	if (data.rounds[round].rows[row][col] == 'M')
		for (var k = 0; k < data.nb_ghost_wall; ++k) {
			if (data.rounds[round].walls[k].i == row 
					&& data.rounds[round].walls[k].j == col )
				return data.rounds[round].walls[k].present
		}
	return false;
}


// *********************************************************************
// Button events
// *********************************************************************

function playButton () {
    gamePaused = false;
    gameAudio = true;
}

function pauseButton () {
    gamePaused = true;
    gamePreview = true; //To call render again
    gameAudio = false;
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

function audio() {

	if (actRound < data.nb_rounds) {
		if (gameAudio) {
			gameAudio = false;
			snds.song.pause();
		}
		else {
			gameAudio = true;
			snds.song.play();
		}
	}
	else
		if (!snds.victory.paused) snds.victory.pause();
		else snds.victory.play();	
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
        writeGameState(0);
        writeGameState(1);
        writeGameState(2);
        writeGameState(3);

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
    var frame_time = 5000/speed;
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
