const fs = require('fs');

var input = fs.readFileSync('input', 'utf8').trim().split('\n');
const inputFirst = input[0].split(',');
const inputSecond = input[1].split(',');


/**
 * Receives a move as a string and an initial position object.
 * Returns a list of string coordinates of all the points it crosses.
 *
 * Note that the position will be modified in the process, and be left at the
 * ending point.
 * */
function doMove(move, pos) {
    var coords = [];
    var count = move.substr(1, move.length);
    for(var i=0; i<count; i++) {
        switch(move[0]) {
            case 'R':
                pos.x++; break;
            case 'L':
                pos.x--; break;
            case 'U':
                pos.y--; break;
            case 'D':
                pos.y++; break;
        }
        coords.push('' + pos.x + ',' + pos.y)
    }

    return coords;
}

// Read all the first wire moves and store all the points it crosses in a set
var pos = {x: 0, y: 0};
var first = {'0,0': 0};
var steps = 0;
inputFirst.forEach(move => {
    var coords = doMove(move, pos);
    coords.forEach(step => {
        steps++;
        if(!first.hasOwnProperty(step)) {
            first[step] = steps;
        }
    });
});

// Read all the moves of the second wire, and for every one of them, check if
// wire 1 is already there, and if so check if the manhattan distance is
// shorter that our previous best.
pos = {x: 0, y: 0};
steps = 0;
var bestPoint = null;
var lowestSteps = Infinity;
inputSecond.forEach(move => {
    var coords = doMove(move, pos);
    coords.forEach(repr => {
        steps++;
        if(first.hasOwnProperty(repr)) {
            totalSteps = steps + first[repr];
            if(totalSteps < lowestSteps) {
                lowestSteps = totalSteps;
                bestPoint = repr;
            }
        }
    });
});

console.log("Best point: " + bestPoint);
console.log("Lowest steps: " + lowestSteps);
