var size;
var texturePrefix = "";

var FIELD;
var OPENED;
var score = 0;
var bscore = 0;
var count = 0;

var lFIELD;
//var lOPENED;
var lscore = 0;
var lbscore = 0;
var lcount = 0;

var coins = 0;
animals = {none : 0, fish : 1, frog : 2, mouse : 3, cat : 4, dog : 5, sheep : 6, pig : 7, cow : 8, horse : 9, rhino : 10, giraffe : 11, elephant : 12, wheal : 13};
var incoming = animals.fish;
var lincoming = animals.fish;

var AddBoostClickEventStop = false;
var PanelItemMouseEventStop = false;
var BoostAdd_On = false;
var BoostDelete_On = false;
var BoostMove_On = false;
var BoostMoveSelect_On = false;
var BoostLock = false;

function CreateField(matrix_size, spacing)
{
    size = matrix_size;
    var field = document.getElementById("field");
    var opened = document.getElementById("opened");
    var fieldSide = parseInt(window.getComputedStyle(field, null).width);
    var squareSide = (fieldSide - spacing)/size - spacing;

    var next = document.getElementById("next");
    next.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)";
    
    FIELD = new Array();
    lFIELD = new Array();
    for(var i = 0; i < size; ++i)
    {
        FIELD[i] = new Array();
        lFIELD[i] = new Array();
        for(var j = 0; j < size; ++j)
        {
            var square = document.createElement("div");
            field.appendChild(square);
            square.id = i + "-" + j;
            square.className = "sSquare";
            square.style.width = square.style.height = squareSide + "px";
            square.style.left = j*squareSide + (j+1)*spacing + "px";
            square.style.top  = i*squareSide + (i+1)*spacing + "px";
            square.style.borderRadius = squareSide/10 + "px";
            square.onclick =  function(){PutAnimal(this.id)};
            square.onmouseover = function(){Highlight(this.id)};
            square.onmouseout = function(){uHighlight(this.id)};
            
            FIELD[i][j] = lFIELD[i][j] = animals.none;
        }
    }
    
    CreateOpenedPanel(fieldSide);
    CreateBoostPanel(fieldSide, spacing);
    
    ReadCookies();
    ShowBestScore();
    ShowScore();
    ShowField();
    ShowOpened();
    ShowCoins();
    if(count == size*size) Foggy();
}

function PutAnimal(ID)
{
    if(AddBoostClickEventStop && !BoostAdd_On)
    {
        Cancel();
        return;
    }
    
    uHighlight(ID);
    var i = ID.split("-");
    i[0] = parseInt(i[0]);
    i[1] = parseInt(i[1]);
    
    if(FIELD[i[0]][i[1]] == animals.none)
    {
        SaveField();
        if(BoostAdd_On || BoostMove_On) 
        {
            var temp = incoming;
            incoming = putAnimal;
            putAnimal = temp;
        }
        
        var square = document.getElementById(ID);
        switch(incoming)
        {
            case animals.fish	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)"; break;
            case animals.frog	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/frog.png)"; break;
            case animals.mouse	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/mouse.png)"; break;
            case animals.cat	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/cat.png)"; break;
            case animals.dog	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/dog.png)"; break;           
            case animals.sheep	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/sheep.png)"; break;
            case animals.pig	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/pig.png)"; break;
            case animals.cow	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/cow.png)"; break;
            case animals.horse	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/horse.png)"; break;
            case animals.rhino	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/rhino.png)"; break;
            case animals.giraffe :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/giraffe.png)"; break;
            case animals.elephant:    square.style.backgroundImage = "url(" + texturePrefix + "Textures/elephant.png)"; break;
            case animals.wheal	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/wheal.png)"; break
            default	 :    break;
        }
        FIELD[i[0]][i[1]] = incoming;
        if(!BoostMove_On) OPENED[incoming - 1] = true;
        if(!BoostMove_On) count ++;
        if(!BoostMove_On) score += Math.pow(3 , incoming - 1);

        if(score > bscore)
        {
            bscore = score;
            ShowBestScore();
        }
        ShowScore();
        ShowCoins();
        ShowOpened();
        
        if(!BoostAdd_On && !BoostMove_On) ChangeIncoming();
        if(BoostAdd_On)
        {
            coins -= Math.pow(3 , incoming - 1);
            ShowCoins();
            var temp = incoming;
            incoming = putAnimal;
            putAnimal = temp;
            Cancel();
        }
        if(BoostMove_On)
        {
            coins -= 40;
            ShowCoins();
            var temp = incoming;
            incoming = putAnimal;
            putAnimal = temp;
            FIELD[movedAnimalIDArray[0]][movedAnimalIDArray[1]] = animals.none;
            ShowField();
            Cancel();
        }
        Check(i[0], i[1]);
    }
    else if((BoostMove_On || BoostAdd_On) && FIELD[i[0]][i[1]] != animals.none) Cancel();
}

function ChangeIncoming()
{
    var rand = parseInt(1000*Math.random());
         if(rand <= 860)		incoming = animals.fish;
    else if(rand > 860 && rand <= 970)	incoming = animals.frog;
    else if(rand > 970 && rand <= 995)	incoming = animals.mouse;
    else if(rand > 995)		incoming = animals.cat;
    
    var next = document.getElementById("next");
    switch(incoming)
    {
        case animals.fish	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)"; break;
        case animals.frog	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/frog.png)"; break;
        case animals.mouse	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/mouse.png)"; break;
        case animals.cat	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/cat.png)"; break;
        default		 :    break;
    }
    WriteCookies();
}

function bfs(x, y, value)
{  
    if(FIELD[x][y] == animals.wheal)
    {
        WriteCookies();
        return;
    }
    var arrX = new Array();
    var arrY = new Array();
    var used = new Array();
    var steps = [[1, 0, -1, 0, 1, 1, -1, -1], [0, 1, 0, -1, 1, -1, 1, -1]];
    var pointer = 0;
    var to = 0;
    
    for(var i = 0 ; i < 10 ; i++)
    {
        arrX[i] = arrY[i] = -1;
    }
    for(var i = 0; i < size; ++i)
    {
        used[i] = new Array();
        for(var j = 0; j < size; ++j)
        {
            used[i][j] = false;
        }
    }

    arrX[pointer] = x;
    arrY[pointer] = y;
    used[x][y] = true;
    while(arrX[pointer] != -1)
    {
        for(var i = 0 ; i < 4 ; ++i)
        if
        (
            arrX [pointer] + steps[0][i] < size &&
            arrX[pointer] + steps[0][i] >= 0 &&
            arrY[pointer] + steps[1][i] < size &&
            arrY[pointer] + steps[1][i] >= 0 && 
            FIELD[arrX[pointer] + steps[0][i]][arrY[pointer] + steps[1][i]] == value &&
            !used[arrX[pointer] + steps[0][i]][arrY[pointer] + steps[1][i]]
        )
        {
            used[arrX[pointer] + steps[0][i]][arrY[pointer] + steps[1][i]] = true ;
            to++;
            arrX[to] = arrX[pointer] + steps[0][i];
            arrY[to] = arrY[pointer] + steps[1][i];
        }
        pointer++;
    }
    
    if(pointer >= 3)
    {
        count -= (pointer - 1);
        for(var i = 0; i < pointer; i++)
        {
            FIELD[arrX[i]][arrY[i]] = animals.none;
            var ID = "";
            ID += arrX[i] + "-" + arrY[i];
            var str = "";
            str += "SCORE: <br>" + score;    
            var temp = document.getElementById("score");
            temp.innerHTML = str;
         
            var square = document.getElementById(ID);
            square.style.backgroundImage = "url(" + texturePrefix + "Textures/square.png)";;
        }
        FIELD[x][y] = value + 1;
        OPENED[value] = true;
        ShowOpened();
        var ID = "";
        ID += x + "-" + y;
        score += Math.pow(3 , FIELD[x][y] - 1);

        if(score > bscore)
        {
            bscore = score;
            ShowBestScore();
        }
        ShowScore();
        ShowCoins();
        
        var square = document.getElementById(ID);
        switch(FIELD[x][y])
        {
            case animals.frog	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/frog.png)"; break;
            case animals.mouse	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/mouse.png)"; break;
            case animals.cat	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/cat.png)"; break;
            case animals.dog	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/dog.png)"; break;           
            case animals.sheep	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/sheep.png)"; break;
            case animals.pig	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/pig.png)"; break;
            case animals.cow	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/cow.png)"; break;
            case animals.horse	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/horse.png)"; break;
            case animals.rhino	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/rhino.png)"; break;
            case animals.giraffe :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/giraffe.png)"; break;
            case animals.elephant:    square.style.backgroundImage = "url(" + texturePrefix + "Textures/elephant.png)"; break;
            case animals.wheal	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/wheal.png)"; break;
            default	 :    break;
        }     
        bfs(x, y, FIELD[x][y]);
    }
    WriteCookies();
}

function Check(putX, putY)
{    
    bfs(putX, putY, FIELD[putX][putY]);
    if(count == size*size) Foggy();
}

function SaveField()
{
    for(var i = 0; i < size; ++i)
    {
        for(var j = 0; j < size; ++j)
        {
            lFIELD[i][j] = FIELD[i][j];
        }
    }
    
    /*for(var i = 0; i < 13; ++i)
    {
        lOPENED[i] = OPENED[i];
    }*/
    
    lincoming = incoming;
    lscore = score;
    lbscore = bscore;
    lcount = count;
}

function CreateOpenedPanel(fieldSide)
{
    var openedSquareSide = (fieldSide - 3)/13 - 1;
    var opened = document.getElementById("opened");
    OPENED = new Array();
    lOPENED = new Array();
    opened.style.height = openedSquareSide + 4 + "px"; 
    for(var i = 0; i < 13; ++i)
    {
        var openedSquare = document.createElement("div");
        opened.appendChild(openedSquare);
        openedSquare.id = i + ":";
        openedSquare.className = "sSquare";
        openedSquare.style.width = openedSquare.style.height = openedSquareSide + "px";
        openedSquare.style.left = i*openedSquareSide + (i+1) + "px";
        openedSquare.style.borderRadius = openedSquareSide/10 + "px";
        openedSquare.style.top = "1px";
        
        OPENED[i] = false;
        //lOPENED[i] = false;
    }   
}

function CreateBoostPanel(fieldSide, spacing)
{   
    var squareSide = (fieldSide - spacing)/size - spacing;
    var field = document.getElementById("field");
    for(var i = 0; i < 6; ++i)
    {
        var boost = document.createElement("div");
        field.appendChild(boost);
        boost.className = "sSquare";
        boost.id = i + "_boost";
        boost.style.width = boost.style.height = 2*squareSide + "px";
        boost.style.top = 2*i*(squareSide - 15) + (i+1) + "px";
        boost.style.borderRadius = squareSide + "px";
        boost.style.right = "-130px";
        boost.style.zIndex = "1";
        boost.style.backgroundColor = "transparent";
        boost.onmouseover = function(){HighlightBoost(this.id)};
        boost.onmouseout = function(){uHighlightBoost(this.id)};
        var price = document.createElement("div");
        boost.appendChild(price);
        price.className = "sPrice";
        price.id = i + "_boostprice";
        price.style.visibility = "hidden";
        switch(i)
        {
            case 0  :
	    boost.style.backgroundImage = "url(" + texturePrefix + "Textures/changeboost.png)";
	    price.style.backgroundImage = "url(" + texturePrefix + "Textures/price10.png)";
	    boost.addEventListener("click", Boost_Change, false); 
	    break;
            case 1  :
	    boost.style.backgroundImage = "url(" + texturePrefix + "Textures/lastboost.png)";
	    price.style.backgroundImage = "url(" + texturePrefix + "Textures/price10.png)";
	    boost.addEventListener("click", Boost_Last, false); ;
	    break;
            case 2  :
	    boost.style.backgroundImage = "url(" + texturePrefix + "Textures/addboost.png)";
	    boost.addEventListener("click", Boost_Add, false); 
	    break;
            case 3  :
	    boost.style.backgroundImage = "url(" + texturePrefix + "Textures/deleteboost.png)";
	    price.style.backgroundImage = "url(" + texturePrefix + "Textures/price20.png)";
	    boost.addEventListener("click", Boost_Delete, false); 
	    break;
            case 4  :
	    boost.style.backgroundImage = "url(" + texturePrefix + "Textures/moveboost.png)";
	    price.style.backgroundImage = "url(" + texturePrefix + "Textures/price40.png)";
	    boost.addEventListener("click", Boost_Move, false); 
	    break;
            case 5  :
	    boost.style.backgroundImage = "url(" + texturePrefix + "Textures/cancel.png)";
	    boost.style.visibility = "hidden";
	    boost.style.width = 2*squareSide + "px";
	    boost.style.height = squareSide + "px";
	    boost.style.border = "none";
	    boost.style.top = 2*i*(squareSide - 10) + (i+1) + "px";
	    boost.addEventListener("click", Cancel, false); 
	    break;
            default  : break;
	    
        }
    }    
}

function CreateAnimalPanel()
{
    AddBoostClickEventStop = true;
    var panel = document.createElement("div");
    var boost = document.getElementById("2_boost");
    var panelItemSize = 40;
    var panelHeight = 0;
    var boostside = parseInt(window.getComputedStyle(boost, null).width);
    var itemCount = 0;
    
    boost.appendChild(panel);
    panel.className = "sPanel";
    panel.id = "panel";
    
    for(var i = 0; i < 13; ++i)
    {
        if(OPENED[i])
        {
            var panelItem = document.createElement("div");
            panel.appendChild(panelItem);
            panelItem.className = "sPanelItem";
            panelItem.id = i + "_panelItem";
            panelItem.style.top = itemCount*panelItemSize + 2 + "px";
            panelItem.style.height = panelItem.style.width = panelItemSize + "px";
            var itemPrice = document.createElement("div");
            panelItem.appendChild(itemPrice);
            itemPrice.className = "sPrice";
            itemPrice.id = i + "_panelItemPrice";
            itemPrice.innerHTML = Math.pow(3, i) + "   $";
            itemPrice.style.visibility = "hidden";
            panelItem.onmouseover = function(){HighlightPanelItem(this.id)};
            panelItem.onmouseout = function(){uHighlightPanelItem(this.id)};
            panelItem.addEventListener("click", AddAnimal, false); 
            switch(i + 1)
            {
	case animals.fish	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)"; break;
	case animals.frog	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/frog.png)"; break;
	case animals.mouse	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/mouse.png)"; break;
	case animals.cat	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/cat.png)"; break;
	case animals.dog	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/dog.png)"; break;           
	case animals.sheep	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/sheep.png)"; break;
	case animals.pig	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/pig.png)"; break;
	case animals.cow	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/cow.png)"; break;
	case animals.horse	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/horse.png)"; break;
	case animals.rhino	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/rhino.png)"; break;
	case animals.giraffe	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/giraffe.png)"; break;
	case animals.elephant	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/elephant.png)"; break;
	case animals.wheal	 :    panelItem.style.backgroundImage = "url(" + texturePrefix + "Textures/wheal.png)"; break;
	default	 	 :    break;
            }
            panelHeight += panelItemSize;
            itemCount ++;
        }
    }
    panel.style.height = panelHeight + 6 + "px";
    panel.style.top = -panelHeight/2 + boostside/2 - 4 + "px";
    if(itemCount == 0) Cancel();
}

function DeleteAnimalPanel()
{
    AddBoostClickEventStop = false;
    var panel = document.getElementById("panel");
    for(var i = 0; i < 13; ++i)
    {
        if(OPENED[i])
        {
            var panelItem = document.getElementById(i + "_panelItem");
            panel.removeChild(panelItem);
        }
    }
    panel.parentNode.removeChild(panel);
}

function CreateExtraField(mode)
{
    var field = document.getElementById("field");
    for(var i = 0; i < size; ++i)
    {
        for(var j = 0; j < size; ++j)
        {           
            var square = document.getElementById(i + "-" + j);
            var highlightdelsquare = square.cloneNode(false);
            highlightdelsquare.id = i + "-" + j + "-exF";
            field.appendChild(highlightdelsquare);
            highlightdelsquare.style.backgroundImage = "none";
            highlightdelsquare.style.zIndex = "100";
            highlightdelsquare.style.backgroundColor = "transparent";
            highlightdelsquare.onmouseover = function(){Highlight(this.id)};
            highlightdelsquare.onmouseout = function(){uHighlight(this.id)};
                 if(mode == "BD") highlightdelsquare.onclick = function(){DeleteAnimal(this.id)};
            else if(mode == "BM") highlightdelsquare.addEventListener("click", SelectAnimal, false); 
        }
    }
}

function CorrectID(ID)
{
    var i = ID.split("-");
    i[0] = parseInt(i[0]);
    i[1] = parseInt(i[1]);
    
    return (i[0] + "-" + i[1]);
}