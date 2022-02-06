// $Id$
var n = 3;
var L = n*n;
var N = L*L;
var no_candidates = new Array(N);
var candidates = new Array(N);
var changes = new Array();

//simple functions meant to be inline
function Row(X){
  return Math.floor(X/L);
}
function Col(X){
  return X%L;
}
function Box(X) {
	return (Math.floor(Row(X)/n)*n + Math.floor(Col(X)/n) + 1);
}


//Global killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {
	
   		$("#sudoku-frame").html('<div id="sudoku-option"><select  id="sudoku-size"><option value="3">9 x 9</option><option value="4">16 x 16</option>    <option value="5">25 x 25</option></select></div><div id="sudoku-display"></div>');
			
			$("#sudoku-size").mouseup(function (e) {
			
				n = this.value;
				L = n*n;
				N = L*L;
				var temptxt = '<div id="sudoku-grid"> You can enter a new puzzle and let the system solve for you or  try our puzzle. Leave the cells blank or enter 0 in the cells to be solved <br />';
				for (var y = 0; y < N;y++) {					
					if (y%L==0)
						temptxt += '<br />';
					temptxt += '<input type="text" name="su" id="s['+y+']" size="1" maxlength="2" class="sudoku">';
				}					
				temptxt += '<br/><br/><input  type="submit" id="sudoku-generate" value="Generate a puzzle"><br/><input  type="submit" id="sudoku-solve" value="Solve"><input  type="submit" id="sudoku-close" value="Close"></div>';
				$("#sudoku-display").html(temptxt).slideDown("slow");	
				var wi = (n==3)?'170px':(n==4)?'300px':(n==5)?'450px':'170px';
				$("#sudoku-display").css("width",wi);
				
				
				$("#sudoku-generate").click(function (e) {
					if (n==3) {
				  	var filled = [9,0,0,6,5,1,0,0,8,0,0,0,0,0,3,6,0,0,0,6,4,9,0,0,7,0,0,5,4,0,3,0,2,1,0,6,2,0,0,0,0,0,0,0,4,6,0,1,8,0,7,0,9,2,0,0,3,0,0,9,8,6,0,0,0,2,5,0,0,0,0,0,1,0,0,7,8,4,0,0,3];
							//var filled = [0,S4,S3,0,0,0,S6,0,0,0,0,0,S6,S9,0,0,0,S5,S8,0,0,0,0,S4,0,0,S7,0,0,S6,S7,0,S2,0,S9,0,0,S2,0,0,S3,0,0,S5,0,0,S1,0,S8,0,S6,S2,0,0,S1,0,0,S9,0,0,0,0,S4,S5,0,0,0,S7,S1,0,0,0,0,0,S2,0,0,0,S7,S8,0];
						//var filled = [S5,0,0,0,0,0,0,0,S8,0,0,0,S5,0,0,0,0,S9,S7,S9,S6,0,S4,0,S1,0,0,S9,0,0,S6,0,S4,0,S1,0,0,S7,0,0,0,0,0,S8,0,0,S3,0,S8,0,S1,0,0,S2,0,0,S1,0,S5,0,S9,S4,S7,S2,0,0,0,0,S7,0,0,0,S3,0,0,0,0,0,0,0,S6];
					  for (i = 0; i < N; i++){
						  if (filled[i])
								$("input[@name=su]").get(i).value = filled [i];					
						}
					}
					else
					alert("sorry currently puzzles are not available for selected grid size");
				});
			
			
			
			$("#sudoku-close").click(function (e) {
				$("#sudoku-grid").hide("slow");
			});
			
			$("#sudoku-solve").click(function (e) {
				var c;var i;
				var filled = new Array(N);
				var message = ' ';;
				for (i = 0; i < N; i++){
					no_candidates[i] = L+1;
					filled [i] = $("input[@name=su]").get(i).value;					
					if(filled[i])
						filled[i] = 1 <<(filled [i] -1);
					else
						filled [i] = 0;
					
				}
				
				//debugging		
				
				fillAllCandidates(filled, no_candidates, candidates);	
				rec_backtracking(filled, no_candidates, candidates, changes);

				for (var j = 0; j < N; j++){
					if (filled[j]==0) {
						alert("Couldn't be solved");
						break;
					}
					for (var k =1; k<=L; k++)
						if(1<<(k-1) & filled[j]){
							$("input[@name=su]").get(j).value = k;
						}
				}
			});
		});
 });
} 



/*
 * function performing recursive backtracking based on CSP with MRV and forward checking
 *
 * @return
 *   -1 = failure
 *	 81 = success
 */
function rec_backtracking(filled, no_candidates, candidates, changes) {
	var t = isComplete(filled, no_candidates);	
	if(t==N)
		return N;
	var c = no_candidates[t];

//alert(t + ' ' + candidates[t]);

	for(var x=1; (x <= L) && c; x++) {
		var temp = 1<<(x-1);
		if (temp & candidates[t]) {
			c--;
			filled[t] = temp;

			//assignment			
			var i = 0; 
			var b = Row(t)*L;
			for(i= b; i< L+b; i++) {
				if (!filled[i]) {
					if (candidates[i]&temp) {
					  
						candidates[i] = candidates[i] & (~temp);
						no_candidates[i]--;
						changes.push(i);
					}
				}
			}
			b = Col(t);
			for(i = b; i< N; i= i+L) {
				if (!filled[i]) {
					if (candidates[i]&temp) {
						candidates[i] = candidates[i] &  (~temp);
						no_candidates[i]--;
						changes.push(i);
					}
				}
			}
			b  = Math.floor((Box(t)-1)/n)*n*L + ((Box(t)-1)%n*n);
			for(i = b; i<b + n*L; i = i + L) {
				for(var k=0; k <n; k++) {
					if (!filled[k+i]) {
						if (candidates[k+i] & temp) {
							candidates[k+i] = candidates[k+ i] &   (~temp);
							no_candidates[k+i]--;
							changes.push(k+i);
						}
					}
				}
			}
			changes.push(~t);
		
			if (rec_backtracking(filled,no_candidates,candidates,changes) != -1) {
				return N;
			}
			else
			{
				//removeAssignmnet
					var t = ~(changes.pop());
					temp = filled[t];
					filled[t] = 0;
					while ( (t=changes.pop()) > 0) {
						candidates[t] = candidates[t] | temp;
						no_candidates[t]++;
					}
					changes.push(t);

			}
		}
	}
	return -1;
}


/*
 * checks for completeness of the puzzle and returns MRV
 *
 * @return
 *   0 - puzzle is completed
 *	 -1 - failure
 *   n - cell with minimum no of candidates
 */
function isComplete(filled, no_candidates) {
	var location = N;
	var minVal = L+1;
	for(var a = 0; a <N; a++) {
		if (!filled[a]) {
			if (minVal > no_candidates[a]) {
				minVal = no_candidates[a];
				location = a;
			}
		}
	}
	return location;
}


/*
 * parent funciton of fillCandidate, calls fillCandidate for every cell
 *
 */
function fillAllCandidates(filled, no_candidates, candidates) {

	for (var location = 0; location< N; location++) {
		if (!filled[location]) {
		
				var c = 0;
				var l = 0;
				var i = 0; 
				//	doing row
				var msg = ' ';
				var b = Row(location)*L;
				
				for(i= b; i< L+b; i++) {
					l = l |  filled[i];
			
				}
					
				//	doing column
				b = Col(location);
				for(i = b; i< N; i= i+L) {
					l = l |   filled[i];
				}

				// doing box
				b  = Math.floor((Box(location)-1)/n)*n*L + ((Box(location)-1)%n*n);
				for(i = b; i<b + n*L; i = i + L) {
					for(var k=0; k <n; k++) {
						l = l | filled[k + i];	
					}
				}			

				l = ~l;
				l = l & ((1 << L)-1);
				for (i = 1; i <= L; i ++)
					if ( l & (1<<(i-1)))
						c++;

				no_candidates[location] = c;
				candidates[location] = l;
		}
	}
	return;
}
