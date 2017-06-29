var putAnimal;
var movedAnimalIDArray = new Array();

function Boost_Change()
{
    if(!BoostLock)
    {
        Cancel();
        var saved_incoming = incoming;
        while(saved_incoming == incoming)
        {
            var rand = parseInt(1000*Math.random());
	if(rand <= 860)		incoming = animals.fish;
            else if(rand > 860 && rand <= 970)	incoming = animals.frog;
            else if(rand > 970 && rand <= 995)	incoming = animals.mouse;
            else if(rand > 995)		incoming = animals.cat;
        }

        var next = document.getElementById("next");
        switch(incoming)
        {
            case animals.fish	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/fish.png)"; break;
            case animals.frog	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/frog.png)"; break;
            case animals.mouse	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/mouse.png)"; break;
            case animals.cat	 :    next.style.backgroundImage = "url(" + texturePrefix + "Textures/cat.png)"; break;
            default		 :    break;
        }    
        coins -= 10;
        WriteCookies();
        ShowCoins();
        Cancel();
    }
}

function Boost_Last()
{
    if(!BoostLock)
    {   
        if(score != 0)
        {
            Cancel();
            var temp;
            
            temp = score;
            score = lscore;
            lscore = temp;
            
            temp = bscore;
            bscore = lbscore;
            lbscore = temp;
            
            temp = count;
            count = lcount;
            lcount = temp;
	    
            temp = incoming;
            incoming = lincoming;
            lincoming = temp;
            
            /*for(var i = 0; i < 13; ++i)
            { 
	temp = OPENED[i];
	OPENED[i] = lOPENED[i];
	lOPENED[i] = temp;
            }*/
            for(var i = 0; i < size; ++i)
            {
	for(var j = 0; j < size; ++j)
	{
	    temp = FIELD[i][j];
	    FIELD[i][j] = lFIELD[i][j];
	    lFIELD[i][j] = temp;
	}
            }
            coins -= 10;
            WriteCookies();
            ShowScore();
            ShowBestScore();
            ShowField();
            ShowCoins();
            ShowOpened();
            Cancel();
        }        
    }
}

function Boost_Add()
{
    if(score > 0)
    {
        if(!AddBoostClickEventStop && !BoostLock)
        {
            Cancel();
            document.getElementById("5_boost").style.visibility = "visible";
            CreateAnimalPanel();
        }
    }
}

function Boost_Delete()
{
    if(!BoostLock)
    {
        if(score != 0)
        {
            Cancel();
            document.getElementById("5_boost").style.visibility = "visible";
            BoostDelete_On = true;
            CreateExtraField("BD");
        }
    }
}

function Boost_Move()
{
    if(!BoostLock)
    {
        if(score != 0)
        {
            Cancel();
            document.getElementById("5_boost").style.visibility = "visible";
            BoostMove_On = BoostMoveSelect_On = true;
            CreateExtraField("BM");
        }
    }
}

function Cancel()
{
    document.getElementById("5_boost").style.visibility = "hidden";
    if(document.getElementById("panel")) DeleteAnimalPanel();
    AddBoostClickEventStop = false;
    PanelItemMouseEventStop = false;
    BoostAdd_On = false;
    BoostDelete_On = false;
    BoostMove_On = false
    BoostMoveSelect_On = false;
    var field = document.getElementById("field");
    for(var i = 0; i < size; ++i)
    {
        for(var j = 0; j < size; ++j)
        {
            if(document.getElementById(i + "-" + j + "-exF"))
            {
	var exFsquare = document.getElementById(i + "-" + j + "-exF");
	field.removeChild(exFsquare);
            }
        }
    }
}

function AddAnimal(e)
{
    PanelItemMouseEventStop = false;
    var panel = document.getElementById("panel");
    var panelWidth = parseInt(window.getComputedStyle(panel, null).width);
    for(var i = 0; i < 13; ++i)
    {
        if(OPENED[i])
        {
            var obj = document.getElementById(i + "_panelItem");
            obj.style.border = "1px solid black";
            if(parseInt(window.getComputedStyle(obj, null).width) >= panelWidth && (i + "_panelItem") != e.target.id)
            {
	uHighlightPanelItem(i + "_panelItem");
            }
        }
    }
    if(parseInt(window.getComputedStyle(e.target, null).width) < panelWidth)
    {
        HighlightPanelItem(e.target.id);
    }
    PanelItemMouseEventStop = true;
    var obj = document.getElementById(e.target.id);
    document.getElementById(e.target.id + "Price").style.visibility = "hidden";
    obj.style.border = "4px solid red";
    
    putAnimal = parseInt(e.target.id.split("_")[0]) + 1;
    BoostAdd_On = true;
}

function DeleteAnimal(ID)
{
    var i = ID.split("-");
    i[0] = parseInt(i[0]);
    i[1] = parseInt(i[1]);
    
    if(FIELD[i[0]][i[1]] == animals.none)
    {
        Cancel();
    }
    else
    {
        SaveField();
        FIELD[i[0]][i[1]] = animals.none;
        document.getElementById(i[0] + "-" + i[1]).style.backgroundImage = "none";
        coins -= 20;
        count --;
        WriteCookies();
        ShowCoins();
        Cancel();
    }
}

function SelectAnimal(e)
{
    var ID = e.target.id;
    var i = ID.split("-");
    i[0] = parseInt(i[0]);
    i[1] = parseInt(i[1]);
    if(FIELD[i[0]][i[1]] == animals.none) 
    {
        Cancel();
    }
    else
    {   
        var extSquare = document.getElementById(ID);
        var eSqaureLeft = parseInt(window.getComputedStyle(extSquare, null).left);
        var eSqaureTop = parseInt(window.getComputedStyle(extSquare, null).top);
        extSquare.style.left = eSqaureLeft - 5 + "px";
        extSquare.style.top = eSqaureTop - 5 + "px";
        extSquare.style.backgroundImage = "none";
        extSquare.style.backgroundColor = "transparent";
        extSquare.style.border = "6px solid red";
        extSquare.style.borderRadius = "16px";        
        extSquare.style.zIndex = "1000";
        uHighlight(ID);
        putAnimal = FIELD[i[0]][i[1]];
        movedAnimalIDArray = i;
        BoostMoveSelect_On = false;
        
        for(var i = 0; i < size; ++i)
        {
            for(var j = 0; j < size; ++j)
            {
	ID = i + "-" + j + "-exF";
	document.getElementById(ID).removeEventListener("click", SelectAnimal, false);
	document.getElementById(ID).addEventListener("click", MoveAnimal, false);
            }
        }
    }
}

function MoveAnimal(e)
{
    PutAnimal(CorrectID(e.target.id));
}