# SeaEliminate-海洋消除，限时计分类消除游戏，使用cocos2d-3.x引擎
##开始游戏后先加载游戏资源，画面限时Loading...，加载完毕后进入游戏主页
###游戏主页<br>
![](https://github.com/qzl1994/SeaEliminate/raw/master/readme/home.png)  <br>
###点击屏幕，进入游戏页面
![](https://github.com/qzl1994/SeaEliminate/raw/master/readme/game.png)  <br>
初始化游戏场景，生成符合规则的动物表格，通过cocos2d的触摸事件机制实现鼠标点击移动<br>
只可以移动触摸精灵的上下左右四个方向，移动完成后如果匹配满足消除条件，则消除掉落新的动物精灵，否则，交换失败，动物移回原来的位置<br>
在页面下方，通过LoadingBar类实现计时条，每秒减2%，如果有消除精灵，则增加时间<br>
右下方则是分数，每当有消除精灵，则加对应分数<br>
游戏时间耗完，游戏结束，进入游戏结束场景
