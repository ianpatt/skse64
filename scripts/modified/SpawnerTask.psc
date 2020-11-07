Scriptname SpawnerTask Hidden

; SpawnerTask allows to spawn and position an arbitrary number references in game world.
; It's effectively a batch combination of PlaceAtMe and SetPosition/MoveTo that smoothly executes in a single frame.
;
; Example:
;
;		ObjectReference player 	 = ...
;		Form    		chair    = ...
;		float[]			offset   = new float[3]
;		float[]			rotation = new float[3]
;	
;		; Allocate new task
;		int taskId = SpawnerTask.Create()
;	
;		; No rotation
;		rotation[0] = 0
;		rotation[1] = 0
;		rotation[2] = 0
;	
;		; Spawn 100 chairs in a grid above the player
;		int i = 0
;		while i < 100
;			offset[0] = -250 + (i / 10) * 50
;			offset[1] = -250 + (i % 10) * 50
;			offset[2] = 200
;	
;			SpawnerTask.AddSpawn(taskId, chair, player, offset, rotation)
;			i += 1
;		endWhile
;
;		; Run the task and return all placed references in an array
;		ObjectReference[] spawned = SpawnerTask.Run(taskId)


; Creates a new SpawnerTask and returns a handle, which is an identifier for the created task.
; The task handle is valid until the task has been run or canceled, or until the calling stack has exited.
;	(Function type: non-delayed)
;
int Function Create() global native

; Adds a spawn to the task identified by the given handle.
; Running the task places a new instance of formToPlace at target reference with given rotation and position offset. Parameters are analogously defined to PlaceAtMe.
; Multiple spawns can be added to the same task to be executed in a batch (which is the purpose).
;	(Function type: non-delayed)
;
Function AddSpawn(int handle, Form formToPlace, ObjectReference target, float[] positionOffset, float[] rotation, int count = 1, bool bForcePersist = false, bool bInitiallyDisabled = false) global native

; Runs the task and returns the spawned references in an array. May return arrays with a size larger than 128.
; The resources allocated to the task are freed in the process, so the same task handle cannot be run twice.
;	(Function type: latent)
;
ObjectReference[] Function Run(int handle) global native

; Cancels a task before running it and frees its allocated resources.
; Tasks cannot be canceled once they have been started with Run, and vice versa.
;
Function Cancel(int handle) global native
