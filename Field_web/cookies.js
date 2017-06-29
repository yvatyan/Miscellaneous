function ReadCookies()
{   
    if(navigator.cookieEnabled)
    {
        if(document.cookie != "")
        {
            var message = document.cookie;
            var arr01, arr02;
            var arr_vars, arr_lvars, arr_open, /*arr_lopen,*/ arr_field, arr_lfield;
            var obj = {};
            if(message.indexOf("; ") != -1)
            {
	arr01 = message.split("; ");
	for(var i = 0; i < arr01.length; ++i)
	{
	    arr02 = arr01[i].split("=");
	    obj[arr02[0]] = arr02[1];
	}
            }
            else
            {
	arr02 = message.split("=");
	obj[arr02[0]] = arr02[1];
            }
            arr_vars = obj.variables.split(":");
            arr_lvars = obj.lastvariables.split(":");
            arr_open = obj.opened.split(":");
            //arr_lopen = obj.lastopened.split(":");
            arr_field = obj.field.split(":");
            arr_lfield = obj.lastfield.split(":");

            bscore = parseInt(arr_vars[0]);
            score = parseInt(arr_vars[1]);
            count = parseInt(arr_vars[2]);
            coins = parseInt(arr_vars[3]);
            incoming = parseInt(arr_vars[4]);
            
            lbscore = parseInt(arr_lvars[0]);
            lscore = parseInt(arr_lvars[1]);
            lcount = parseInt(arr_lvars[2]);
            lincoming = parseInt(arr_lvars[3]);
            
            for(var i = 0; i < 13; ++i)
            {
	OPENED[i] = (arr_open[i] === "true");
	//lOPENED[i] = (arr_lopen[i] === "true");
            }
            
            for(var i = 0; i < size; ++i)
            {
	for(var j = 0; j < size; ++j)
	{
	    FIELD[i][j] = parseInt(arr_field[i*size + j]);
	    lFIELD[i][j] = parseInt(arr_lfield[i*size + j]);
	}
            }
        }
    }
    NoCookies();
}

function WriteCookies()
{
    if(navigator.cookieEnabled)
    {
        DeleteCookies();
        var endDate = new Date();
        endDate.setTime(endDate.getTime() + 1000*60*60*24*31); // 1 month
        endDate = endDate.toGMTString();
        
        var message = "variables=" + bscore + ":" + score + ":" + count + ":" + coins + ":" + incoming;
        message += "; expires=" + endDate + ";";
        document.cookie = message;

        var message = "lastvariables=" + lbscore + ":" + lscore + ":" + lcount + ":" + lincoming;
        message += "; expires=" + endDate + ";";
        document.cookie = message;
        
        message = "field=";
        for(var i = 0; i < size; ++i)
        {
            for(var j = 0; j < size; ++j)
            {
	message += FIELD[i][j] + ":";
            }
        }
        message += "; expires=" + endDate + ";";
        document.cookie = message;
        
        message = "lastfield=";
        for(var i = 0; i < size; ++i)
        {
            for(var j = 0; j < size; ++j)
            {
	message += lFIELD[i][j] + ":";
            }
        }
        message += "; expires=" + endDate + ";";
        document.cookie = message;
        
        message = "opened=";
        for(var i = 0; i < 13; ++i)
        {
            message += OPENED[i] + ":";
        }
        message += "; expires=" + endDate + ";";
        document.cookie = message;
        
        /*message = "lastopened=";
        for(var i = 0; i < 13; ++i)
        {
            message += lOPENED[i] + ":";
        }
        message += "; expires=" + endDate + ";";
        document.cookie = message;*/
    }
    NoCookies();
}

function DeleteCookies()
{
    if(navigator.cookieEnabled)
    {
        if(document.cookie != "")
        {
            var endDate = new Date();
            endDate.setTime(1000);
            endDate = endDate.toGMTString();
            document.cookie = "variables=; expires=" + endDate + ";";
            document.cookie = "lastvariables=; expires=" + endDate + ";";
            document.cookie = "opened=; expires=" + endDate + ";";
            //document.cookie = "lastopened=; expires=" + endDate + ";";
            document.cookie = "field=; expires=" + endDate + ";";
            document.cookie = "lastfield=; expires=" + endDate + ";";
            ReadCookies();
        }
    }
    NoCookies();
}

function NoCookies()
{
    var warning = document.getElementById("warning");
    if(!navigator.cookieEnabled)
    {       
        warning.style.visibility = "visible";
    }
    else
    {
        warning.style.visibility = "hidden";        
    }
}
