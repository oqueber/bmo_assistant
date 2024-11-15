Here is a simple flow chart:

```mermaid
flowchart TB
	btn_irq@{ shape: trap-t, label: "Button" }
	btn_irq-->btn_press{Short}
	
	btn_press-- Yes -->btn_short
	btn_press-- No -->btn_long
	
	btn_short@{ shape: subproc, label: "btn_short" }
	btn_long@{ shape: subproc, label: "btn_long" }
	btn_short-->loop
	btn_long-->loop

	Power_on@{ shape: circle, label: "Start" }
	Power_on --> Setup

	Setup@{ shape: subproc, label: "Setup" }
	Setup-- Config System -->loop
	subgraph loop [Loop]
		index_select_animation{Animation}
		index_select_animation-- 0 -->face_disable@{ shape: subproc, label: "face_disable" }
		index_select_animation-- 1 -->face_angry@{ shape: subproc, label: "face_angry" }
		index_select_animation-- 2 -->face_looking@{ shape: subproc, label: "face_looking" }
		index_select_animation-- 3 -->face_nervous@{ shape: subproc, label: "face_nervous" }
	end
```