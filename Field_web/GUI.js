function ResetField()
{
    BoostLock = false;
    lcount = count = 0;
    lscore = score = 0;
    lincoming = incoming = animals.fish;
    for(var i = 0; i < size; ++i)
    {
        for(var j = 0; j < size; ++j)
        {
            FIELD[i][j] = lFIELD[i][j] = animals.none;
            var ID = "";
            ID += i + "-" + j;
            var square = document.getElementById(ID);
            square.style.backgroundImage = "none";
        }
    }
    for(var i = 0; i < 13; ++i)
    {
        OPENED[i] = false;
        //lOPENED[i] = false;
    }
    var next = document.getElementById("next");
    next.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)";
    Cancel();
    WriteCookies();
    ShowScore();
    ShowOpened();
}

function ResetBestScore()
{
    lbscore = bscore = 0;
    Cancel();
    WriteCookies();
    ShowBestScore();
}

function ShowScore()
{
    var str = "";
    str += "SCORE: <br>" + score;
    
    var temp = document.getElementById("score");
    temp.innerHTML = str;	
}

function ShowBestScore()
{
    var str = "";
    str += "BEST SCORE: <br>" + bscore;
    var temp = document.getElementById("bScore");
    temp.innerHTML = str;    
}

function Highlight(ID)
{
    var i = ID.split("-");
    i[0] = parseInt(i[0]);
    i[1] = parseInt(i[1]);
    
    if(!BoostDelete_On && !BoostMove_On)
    {
        if(FIELD[i[0]][i[1]] == animals.none)
        {
            var square = document.getElementById(ID);
            square.className = "sHiSquare";
            square.style.backgroundImage = "url(" + texturePrefix + "Textures/highlight.png)";
        }
    }
    else if(BoostDelete_On && !BoostMove_On)
    {
        if(FIELD[i[0]][i[1]] != animals.none)
        {
            document.getElementById(ID).style.backgroundImage = "url(" + texturePrefix + "Textures/highlightdel.png)";
        }
    }
    else if(!BoostDelete_On && BoostMove_On)
    {
        if(FIELD[i[0]][i[1]] != animals.none && BoostMoveSelect_On)
        {
            
            document.getElementById(ID).style.backgroundImage = "url(" + texturePrefix + "Textures/highlightselect.png)";
        }
        else if(FIELD[i[0]][i[1]] == animals.none && !BoostMoveSelect_On)
        {
            document.getElementById(ID).style.backgroundImage = "url(" + texturePrefix + "Textures/highlighttomove.png)";
        }
    }
}

function uHighlight(ID)
{
    var i = ID.split("-");
    i[0] = parseInt(i[0]);
    i[1] = parseInt(i[1]);
    if(!BoostDelete_On && !BoostMove_On)
    {
        if(FIELD[i[0]][i[1]] == animals.none)
        {
            var square = document.getElementById(ID);
            square.className = "sSquare";
            square.style.backgroundImage = "url(" + texturePrefix + "Textures/square.png)";
        }
    }
    else if(BoostDelete_On && !BoostMove_On)
    {
        if(FIELD[i[0]][i[1]] != animals.none && i[2] == "exF")
        {
            document.getElementById(ID).style.backgroundImage = "none";
        }       
    }
    else if(!BoostDelete_On && BoostMove_On)
    {
        if(FIELD[i[0]][i[1]] != animals.none && BoostMoveSelect_On)
        {           
            document.getElementById(ID).style.backgroundImage = "none";
        }
        else if(FIELD[i[0]][i[1]] == animals.none && !BoostMoveSelect_On)
        {
            document.getElementById(ID).style.backgroundImage = "none";
        }
    }
}

function ShowField()
{
    for(var i = 0; i < size; ++i)
    {
        for(var j = 0; j < size; ++j)
        {
            var ID = i + "-" + j;
            var square = document.getElementById(ID);
            switch(FIELD[i][j])
            {
	case animals.none	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/square.png)"; break;
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
	case animals.giraffe 	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/giraffe.png)"; break;
	case animals.elephant	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/elephant.png)"; break;
	case animals.wheal	 :    square.style.backgroundImage = "url(" + texturePrefix + "Textures/wheal.png)"; break;
	default	 	 :    break;
            }
        }
    }
    var next = document.getElementById("next");
    switch(incoming)
    {
        case animals.fish	:    next.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)"; break;
        case animals.frog	:    next.style.backgroundImage = "url(" + texturePrefix + "Textures/frog.png)"; break;
        case animals.mouse	:    next.style.backgroundImage = "url(" + texturePrefix + "Textures/mouse.png)"; break;
        case animals.cat	:    next.style.backgroundImage = "url(" + texturePrefix + "Textures/cat.png)"; break;
        default		:    break;
    }
}

function Foggy()
{
    var fog = document.getElementById("fog");
    fog.style.visibility = "visible";
    Cancel();
    BoostLock = true;
}

function TryAgain()
{
    var fog = document.getElementById("fog");
    fog.style.visibility = "hidden";
    ResetField();
}

function ShowOpened()
{
    for(var i = 0; i < 13; ++i)
    {
        var openedSquare = document.getElementById(i + ":");
        
        if(OPENED[i])
        {
            switch(i)
            {
	case 0	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)"; break;
	case 1	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/frog.png)"; break;
	case 2	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/mouse.png)"; break;
	case 3	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/cat.png)"; break;
	case 4	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/dog.png)"; break;
	case 5	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/sheep.png)"; break;
	case 6	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/pig.png)"; break;
	case 7	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/cow.png)"; break;
	case 8	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/horse.png)"; break;
	case 9	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/rhino.png)"; break;
	case 10 	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/giraffe.png)"; break;
	case 11	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/elephant.png)"; break;
	case 12	 :    openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/wheal.png)"; break;
	default 	 :    break;	
            }
        }
        else
        {
            openedSquare.style.backgroundImage = "url(" + texturePrefix + "Textures/unknown.png)";
        }
    }
}

function HighlightBoost(ID)
{
    if(!BoostLock)
    {
        document.getElementById(ID).style.zIndex = "10";
        document.getElementById(ID).style.cursor = "pointer";
        if(ID != "2_boost" && ID != "5_boost") document.getElementById(ID + "price").style.visibility = "visible";
    }
    else
    {
        document.getElementById(ID).style.cursor = "default";
    }
}

function uHighlightBoost(ID)
{
    document.getElementById(ID).style.zIndex = 0;
    document.getElementById(ID + "price").style.visibility = "hidden";
}

function ShowCoins()
{
    document.getElementById("coins").innerHTML = coins + "";
}

function HighlightPanelItem(ID)
{
    if(!PanelItemMouseEventStop)
    {
        var animal = document.getElementById(ID);
        animal.style.zIndex = "10";
        document.getElementById(ID + "Price").style.visibility = "visible";
        var animalSize = parseInt(window.getComputedStyle(animal, null).width);
        animal.style.width = animal.style.height = 1.4*animalSize + "px";
        animal.style.left = -(1.4*animalSize/2 - animalSize/2.0) + 2.0 + "px";
        var animalTop = parseInt(window.getComputedStyle(animal, null).top);
        animal.style.top = animalTop - (1.4*animalSize/2.0 - animalSize/2.0) + "px";
    }
}

function uHighlightPanelItem(ID)
{
    if(!PanelItemMouseEventStop)
    {
        var animal = document.getElementById(ID);
        animal.style.zIndex = "0";
        document.getElementById(ID + "Price").style.visibility = "hidden";
        var animalSize = parseInt(window.getComputedStyle(animal, null).width);
        animal.style.width = animal.style.height = animalSize/1.4 + "px";
        animal.style.left = 2 + "px";
        var animalTop = parseInt(window.getComputedStyle(animal, null).top);
        animal.style.top = animalTop + (1.4*animalSize/2.0 - animalSize/2.0) - 3.0 + "px";
    }
}

function SwitchTextures()
{
    if(texturePrefix == "Extended_")
    {
        texturePrefix = "";
        document.getElementById("switcher").innerHTML = "Common";
        for(var i = 0; i < 5; ++i)
        {
            document.getElementById(i + "_boost").style.border = "1px solid black";
        }
        document.getElementById("body").style.backgroundImage = "url(Textures/background.jpg)";
    }
    else if(texturePrefix == "")
    {
        texturePrefix = "Extended_";
        document.getElementById("switcher").innerHTML = "Extended";
        for(var i = 0; i < 6; ++i)
        {
            document.getElementById(i + "_boost").style.border = "none";
        }
        document.getElementById("body").style.backgroundImage = "url(Extended_Textures/background.jpg)";
    }
    
    var elements = new Array();
    elements = document.getElementsByTagName("div");
    
    for(var i = 0, limit = elements.length; i < limit; ++i)
    {
        var link = window.getComputedStyle(elements[i], false).backgroundImage;
        var image = link.split("_web/")[1];
        
        if(image && image != "none")
        {
            image = image.replace("\")", "");
            image = image.replace("Extended_", "");
            elements[i].style.backgroundImage = "url(" + texturePrefix + image + ")";
        }
    }
}