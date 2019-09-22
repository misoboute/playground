import QtQuick 2.0

Item {
    id:zzMain
    width:parent.width
    height: parent.height
    rotation: 0
    
    Canvas {
        id:canvas
        anchors.fill: parent
        antialiasing: true
        
        property double rwidth : parent.width/2
        property double rheight : parent.height/2
        property int lineW: 20
                
        onPaint: {
            
            var ctx = canvas.getContext('2d');
            
            ctx.clearRect(0, 0, width, height)
            
            ctx.beginPath()
            
            ctx.moveTo(rwidth*3/2, rheight)
            ctx.arc(rwidth, rheight, rwidth/2, 0, Math.PI*2, true)
            ctx.lineWidth = lineW
            ctx.strokeStyle = "#444444"
            ctx.stroke()
            
            ctx.beginPath()
              //TODO: should fix the moveto error  

                ctx.moveTo( (canvas.rwidth/2)*Math.cos((nightHandle.rotation-90)*Math.PI/180) + canvas.rwidth, 
                          (canvas.rwidth/2)*Math.sin((nightHandle.rotation-90)*Math.PI/180) + canvas.rheight )
                ctx.arc(rwidth, rheight, rwidth/2, (nightHandle.rotation-90)*Math.PI/180, (dayHandle.rotation-90)*3.14/180, false)

            ctx.lineWidth = 4;
            ctx.strokeStyle = "#fff"
            ctx.stroke()
            
            ctx.restore()
        }
        
        Rectangle {
            id: nightHandle
            x: canvas.rwidth
            y: canvas.rheight-(canvas.rheight/2 + canvas.lineW)
            width: 5
            height: 25
            opacity: 0.5
            transform: Translate{x:-nightHandle.width/2; y:-nightHandle.height/2}
            Rectangle {
                color: "red"
                width: 5
                height: 5
                x:0
                y:0
            }
            MouseArea {
                id: nightHandleMouse
                anchors.fill: parent
                hoverEnabled: true
            }
        } 
        Rectangle {
            id: dayHandle
            x: canvas.rwidth
            y: canvas.rheight+(canvas.rheight/2 - canvas.lineW)
            width: 5
            height:25
            opacity: 0.5
            
            transform: Translate{x:-dayHandle.width/2; y:-dayHandle.height/2}
            Rectangle{
                color: "red"
                width: 5
                height: 5
                x:0
                y:0
            }
            
            MouseArea {
                id: dayHandleMouse
                anchors.fill: parent
                hoverEnabled: true
            }            
        } 
        Rectangle {
            id: zero
            x: canvas.rwidth
            y:canvas.rheight - canvas.rwidth/2
            width: 3
            height: 20
            color: "purple"
        }  
        Rectangle {
            id: six
            x: 3*canvas.rwidth/2 
            y:canvas.rheight
            width: 20
            height: 3
            color: "orange"
        } 
        Rectangle {
            id: twelve
            x: canvas.rwidth
            y:canvas.rheight + canvas.rwidth/2
            width: 3
            height: 20
            color: "green"
        }
        Rectangle {
            id: eighteen
            x: canvas.rwidth/2
            y:canvas.rheight
            width: 20
            height: 3
            color: "blue"
        }
    }
    
    MouseArea {
        id:nMouseMain
        anchors.fill: parent
        
        onMouseXChanged: {
            if(nightHandleMouse.containsMouse){
                nightHandle.rotation = Math.atan2(mouseX-canvas.rwidth, canvas.rheight-mouseY)*180/3.14;
                nightHandle.x = (canvas.rwidth/2)*Math.cos((nightHandle.rotation-90)*3.14/180) + canvas.rwidth;
                nightHandle.y = (canvas.rwidth/2)*Math.sin((nightHandle.rotation-90)*3.14/180) + canvas.rheight;
                canvas.requestPaint();
                console.log("n: ",nightHandle.rotation);
            }
            else if(dayHandleMouse.containsMouse){
                dayHandle.rotation = Math.atan2(mouseX-canvas.rwidth, canvas.rheight-mouseY)*180/3.14;
                dayHandle.x = (canvas.rwidth/2)*Math.cos((dayHandle.rotation-90)*3.14/180) + canvas.rwidth;
                dayHandle.y = (canvas.rwidth/2)*Math.sin((dayHandle.rotation-90)*3.14/180) + canvas.rheight;
                canvas.requestPaint();
                console.log("d: ",nightHandle.rotation);
            }
            while(dayHandle.rotation<0) dayHandle.rotation+=360;
            while(nightHandle.rotation<0) nightHandle.rotation+=360;
        }
        onMouseYChanged: {
            if(nightHandleMouse.containsMouse){
                nightHandle.rotation = Math.atan2(mouseX-canvas.rwidth, canvas.rheight-mouseY)*180/3.14;
                nightHandle.x = (canvas.rwidth/2)* Math.cos((nightHandle.rotation-90)*3.14/180) + canvas.rwidth;
                nightHandle.y = (canvas.rwidth/2)*Math.sin((nightHandle.rotation-90)*3.14/180) + canvas.rheight;
                canvas.requestPaint();            
            }
            else if(dayHandleMouse.containsMouse){
                dayHandle.rotation = Math.atan2(mouseX-canvas.rwidth, canvas.rheight-mouseY)*180/3.14;
                dayHandle.x = (canvas.rwidth/2)*Math.cos((dayHandle.rotation-90)*3.14/180) + canvas.rwidth;
                dayHandle.y = (canvas.rwidth/2)*Math.sin((dayHandle.rotation-90)*3.14/180) + canvas.rheight;                
                canvas.requestPaint();
            }
            while(dayHandle.rotation<0) dayHandle.rotation+=360;
            while(nightHandle.rotation<0) nightHandle.rotation+=360;
        }   
    }   
}
