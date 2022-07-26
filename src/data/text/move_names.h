const u8 gMoveNames[][13] = {
    _("-"),
    _("拍击"),
    _("空手劈"),
    _("连环巴掌"),
    _("连续拳"),
    _("百万吨重拳"),
    _("聚宝功"),
    _("火焰拳"),
    _("冰冻拳"),
    _("雷电拳"),
    _("抓"),
    _("夹住"),
    _("断头钳"),
    _("旋风刀"),
    _("剑舞"),
    _("居合斩"),
    _("起风"),
    _("翅膀攻击"),
    _("吹飞"),
    _("飞翔"),
    _("绑紧"),
    _("摔打"),
    _("藤鞭"),
    _("踩踏"),
    _("二连踢"),
    _("百万吨重踢"),
    _("飞踢"),
    _("回旋踢"),
    _("扔沙"),
    _("头锤"),
    _("角撞"),
    _("乱击"),
    _("角钻"),
    _("撞击"),
    _("泰山压顶"),
    _("紧束"),
    _("猛撞"),
    _("大闹一番"),
    _("舍身冲撞"),
    _("摇尾巴"),
    _("毒针"),
    _("双针"),
    _("飞弹针"),
    _("瞪眼"),
    _("咬住"),
    _("叫声"),
    _("吼叫"),
    _("唱歌"),
    _("超音波"),
    _("音爆"),
    _("定身法"),
    _("溶解液"),
    _("火花"),
    _("喷射火焰"),
    _("白雾"),
    _("水枪"),
    _("水炮"),
    _("冲浪"),
    _("冰冻光束"),
    _("暴风雪"),
    _("幻象光线"),
    _("泡沫光线"),
    _("极光束"),
    _("破坏光线"),
    _("啄"),
    _("啄钻"),
    _("地狱翻滚"),
    _("踢倒"),
    _("双倍奉还"),
    _("地球上投"),
    _("怪力"),
    _("吸取"),
    _("超级吸取"),
    _("寄生种子"),
    _("生长"),
    _("飞叶快刀"),
    _("日光束"),
    _("毒粉"),
    _("麻痹粉"),
    _("催眠粉"),
    _("花瓣舞"),
    _("吐丝"),
    _("龙之怒"),
    _("火焰旋涡"),
    _("点击"),
    _("十万伏特"),
    _("电磁波"),
    _("打雷"),
    _("落石"),
    _("地震"),
    _("地裂"),
    _("挖洞"),
    _("剧毒"),
    _("念力"),
    _("精神强念"),
    _("催眠术"),
    _("瑜伽姿势"),
    _("高速移动"),
    _("电光一闪"),
    _("愤怒"),
    _("瞬间移动"),
    _("黑夜魔影"),
    _("模仿"),
    _("刺耳声"),
    _("影子分身"),
    _("自我再生"),
    _("变硬"),
    _("变小"),
    _("烟幕"),
    _("奇异之光"),
    _("缩入壳中"),
    _("变圆"),
    _("屏障"),
    _("光墙"),
    _("黑雾"),
    _("反射壁"),
    _("聚气"),
    _("忍耐"),
    _("挥指"),
    _("鹦鹉学舌"),
    _("自爆"),
    _("炸蛋"),
    _("舌舔"),
    _("浊雾"),
    _("污泥攻击"),
    _("骨棒"),
    _("大字爆炎"),
    _("攀瀑"),
    _("贝壳夹击"),
    _("高速星星"),
    _("火箭头锤"),
    _("尖刺加农炮"),
    _("缠绕"),
    _("瞬间失忆"),
    _("折弯汤匙"),
    _("生蛋"),
    _("飞膝踢"),
    _("大蛇瞪眼"),
    _("食梦"),
    _("毒瓦斯"),
    _("投球"),
    _("吸血"),
    _("恶魔之吻"),
    _("神鸟猛击"),
    _("变身"),
    _("泡沫"),
    _("迷昏拳"),
    _("蘑菇孢子"),
    _("闪光"),
    _("精神波"),
    _("跃起"),
    _("溶化"),
    _("蟹钳锤"),
    _("大爆炸"),
    _("乱抓"),
    _("骨头回力镖"),
    _("睡觉"),
    _("岩崩"),
    _("必杀门牙"),
    _("棱角化"),
    _("纹理"),
    _("三重攻击"),
    _("愤怒门牙"),
    _("劈开"),
    _("替身"),
    _("挣扎"),
    _("写生"),
    _("三连踢"),
    _("小偷"),
    _("蛛网"),
    _("心之眼"),
    _("恶梦"),
    _("火焰轮"),
    _("打鼾"),
    _("诅咒"),
    _("抓狂"),
    _("纹理2"),
    _("气旋攻击"),
    _("棉孢子"),
    _("起死回生"),
    _("怨恨"),
    _("细雪"),
    _("守住"),
    _("音速拳"),
    _("鬼面"),
    _("出奇一击"),
    _("天使之吻"),
    _("腹鼓"),
    _("污泥炸弹"),
    _("掷泥"),
    _("章鱼桶炮"),
    _("撒菱"),
    _("电磁炮"),
    _("识破"),
    _("同命"),
    _("灭亡之歌"),
    _("冰冻之风"),
    _("看穿"),
    _("骨棒乱打"),
    _("锁定"),
    _("逆鳞"),
    _("沙暴"),
    _("终极吸取"),
    _("挺住"),
    _("撒娇"),
    _("滚动"),
    _("点到为止"),
    _("虚张声势"),
    _("喝牛奶"),
    _("电光"),
    _("连斩"),
    _("钢翼"),
    _("黑色目光"),
    _("迷人"),
    _("梦话"),
    _("治愈铃声"),
    _("报恩"),
    _("礼物"),
    _("迁怒"),
    _("神秘守护"),
    _("分担痛楚"),
    _("神圣之火"),
    _("震级"),
    _("爆裂拳"),
    _("超级角击"),
    _("龙息"),
    _("接棒"),
    _("再来一次"),
    _("追打"),
    _("高速旋转"),
    _("甜甜香气"),
    _("铁尾"),
    _("金属爪"),
    _("借力摔"),
    _("晨光"),
    _("光合作用"),
    _("月光"),
    _("觉醒力量"),
    _("十字劈"),
    _("龙卷风"),
    _("求雨"),
    _("大晴天"),
    _("咬碎"),
    _("镜面反射"),
    _("自我暗示"),
    _("神速"),
    _("原始之力"),
    _("暗影球"),
    _("预知未来"),
    _("碎岩"),
    _("潮旋"),
    _("围攻"),
    _("击掌奇袭"),
    _("吵闹"),
    _("蓄力"),
    _("喷出"),
    _("吞下"),
    _("热风"),
    _("冰雹"),
    _("无理取闹"),
    _("吹捧"),
    _("鬼火"),
    _("临别礼物"),
    _("硬撑"),
    _("真气拳"),
    _("清醒"),
    _("看我嘛"),
    _("自然之力"),
    _("充电"),
    _("挑衅"),
    _("帮助"),
    _("戏法"),
    _("扮演"),
    _("祈愿"),
    _("借助"),
    _("扎根"),
    _("蛮力"),
    _("魔法反射"),
    _("回收利用"),
    _("报复"),
    _("劈瓦"),
    _("哈欠"),
    _("拍落"),
    _("蛮干"),
    _("喷火"),
    _("特性互换"),
    _("封印"),
    _("焕然一新"),
    _("怨念"),
    _("抢夺"),
    _("秘密之力"),
    _("潜水"),
    _("猛推"),
    _("保护色"),
    _("萤火"),
    _("洁净光芒"),
    _("薄雾球"),
    _("羽毛舞"),
    _("摇晃舞"),
    _("火焰踢"),
    _("玩泥巴"),
    _("冰球"),
    _("尖刺臂"),
    _("偷懒"),
    _("巨声"),
    _("剧毒牙"),
    _("撕裂爪"),
    _("爆炸烈焰"),
    _("加农水炮"),
    _("彗星拳"),
    _("惊吓"),
    _("气象球"),
    _("芳香治疗"),
    _("假哭"),
    _("空气利刃"),
    _("过热"),
    _("气味侦测"),
    _("岩石封锁"),
    _("银色旋风"),
    _("金属音"),
    _("草笛"),
    _("挠痒"),
    _("宇宙力量"),
    _("喷水"),
    _("信号光束"),
    _("暗影拳"),
    _("神通力"),
    _("冲天拳"),
    _("流沙地狱"),
    _("绝对零度"),
    _("浊流"),
    _("种子机关枪"),
    _("燕返"),
    _("冰锥"),
    _("铁壁"),
    _("挡路"),
    _("长嚎"),
    _("龙爪"),
    _("疯狂植物"),
    _("健美"),
    _("弹跳"),
    _("泥巴射击"),
    _("毒尾"),
    _("渴望"),
    _("伏特攻击"),
    _("魔法叶"),
    _("玩水"),
    _("冥想"),
    _("叶刃"),
    _("龙之舞"),
    _("岩石爆击"),
    _("电击波"),
    _("水之波动"),
    _("破灭之愿"),
    _("精神突进"),
    _("Leech Fang"),
    _("空气斩"),
    _("水流喷射"),
    _("水流尾"),
    _("波导弹"),
    _("雪崩"),
    _("勇鸟猛攻"),
    _("虫鸣"),
    _("子弹拳"),
    _("充电光束"),
    _("近身战"),
    _("十字毒刃"),
    _("恶之波动"),
    _("放电"),
    _("流星群"),
    _("龙之波动"),
    _("吸取拳"),
    _("大地之力"),
    _("能量球"),
    _("真气弹"),
    _("发劲"),
    _("终极冲击"),
    _("打草结"),
    _("垃圾射击"),
    _("臂锤"),
    _("双刃头锤"),
    _("冰砾"),
    _("铁头"),
    _("喷烟"),
    _("磁铁炸弹"),
    _("泥巴炸弹"),
    _("诡计"),
    _("暗袭要害"),
    _("奇异之风"),
    _("力量宝石"),
    _("强力鞭打"),
    _("精神利刃"),
    _("攀岩"),
    _("岩石打磨"),
    _("岩石炮"),
    _("羽栖"),
    _("种子炸弹"),
    _("暗影爪"),
    _("影子偷袭"),
    _("真空波"),
    _("十字剪"),
    _("意念头锤"),
    _("掉包"),
    _("直冲钻"),
    _("重踏"),
    _("电网"),
    _("蓄能焰袭"),
    _("冰息"),
    _("爱心印章"),
    _("磨爪"),
    _("木角"),
    _("盘蜷"),
    _("暴风"),
    _("冰柱坠击"),
    _("自我激励"),
    _("蝶舞"),
    _("青草搅拌器"),
    _("下盘踢"),
    _("大声咆哮"),
    _("虫之抵抗"),
    _("疯狂滚压"),
    _("山岚摔"),
    _("毒液冲击"),
    _("疯狂伏特"),
    _("毒击"),
    _("酸液炸弹"),
    _("火之舞"),
    _("清除之烟"),
    _("飞叶风暴"),
    _("尖石攻击"),
    _("以牙还牙"),
    _("火焰牙"),
    _("冰冻牙"),
    _("雷电牙"),
    _("闪焰冲锋"),
    _("炼狱"),
    _("祸不单行"),
    _("二连劈"),
    _("二连击"),
    _("棉花防守"),
    _("杂技"),
    _("烈焰溅射"),
    _("急速折返"),
    _("伏特替换"),
    _("龙尾"),
    _("巴投"),
    _("镜光射击"),
    _("木槌"),
    _("治愈波动"),
    _("加农光炮"),
    _("虫咬"),
    _("啄食"),
    _("破壳"),
    _("愤怒粉"),
    _("唤醒巴掌"),
    _("贝壳刃"),
    _("仿效"),
    _("奇迹之眼"),
    _("突袭"),
    _("热水"),
    _("盐水"),
    _("妖精之风"),
    _("圆瞳"),
    _("嬉闹"),
    _("月亮之力"),
    _("吸取之吻"),
    _("魔法闪耀"),
    _("魅惑之音"),
    _("隐形岩"),
    _("毒菱"),
    _("治愈之愿"),
    _("新月舞"),
    _("顺风"),
    _("爆音波"),
    _("烧尽"),
    _("烦恼种子"),
    _("胃液"),
    _("大地掌控"),
    _("鲜花防守"),
    _("耕地"),
    _("换挡"),
    _("黏黏网"),
    _("辅助力量"),
    _("点穴"),
    _("惩罚"),
    _("恶意追击"),
    _("龙之俯冲"),
    _("暗黑洞"),
    _("佯攻"),
    _("潜灵奇袭"),
    _("落英缤纷"),
    _("和睦相处"),
    _("时光咆哮"),
    _("亚空裂斩"),
    _("熔岩风暴"),
    _("搏命"),
    _("暗影潜袭"),
    _("蹭蹭脸颊"),
    _("报仇"),
    _("金属爆破"),
    _("制裁光砾"),
    _("冷冻干燥"),
    _("精神冲击"),
    _("轮唱"),
    _("扫尾拍打"),
    _("齿轮飞盘"),
    _("自然之恩"),
    _("逐步击破"),
    _("击落"),
    _("异次元洞"),
    _("陀螺球"),
    _("异次元猛攻"),
    _("攻击指令"),
    _("防御指令"),
    _("回复指令"),
    _("诱惑"),
    _("清除浓雾"),
    _("根源波动"),
    _("断崖之剑"),
    _("防守互换"),
    _("蒸汽爆炸"),
    _("金属爆炸"),
    _("抢先一步"),
    _("力量互换"),
    _("力量戏法"),
    _("精神转移"),
    _("绞紧"),
    _("王者盾牌"),
    _("精神击破"),
    _("身体轻量化"),
    _("钻丝风暴"),
    _("污泥波"),
    _("重磅冲撞"),
    _("同步干扰"),
    _("电球"),
    _("欺诈"),
    _("找伙伴"),
    _("暗黑爆破"),
    _("回声"),
    _("大地神力"),
    _("死亡之翼"),
    _("心灵互换"),
    _("捏碎"),
    _("圣剑"),
    _("高温重压"),
    _("爆炸头突击"),
    _("高科技光炮"),
    _("古老之歌"),
    _("神秘之剑"),
    _("冰封世界"),
    _("雷击"),
    _("青焰"),
    _("冰冻伏特"),
    _("极寒冷焰"),
    _("V热焰"),
    _("交错火焰"),
    _("交错闪电"),
    _("致命针刺"),
    _("战吼"),
    _("画龙点睛"),
    _("抛下狠话"),
    _("颠倒"),
    _("核心惩罚者"),
    _("喋喋不休"),
    _("密语"),
    _("飞水手里剑"),
    _("魔法火焰"),
    _("尖刺防守"),
    _("怪异电波"),
    _("手下留情"),
    _("死缠烂打"),
    _("增强拳"),
    _("千箭齐发"),
    _("千波激荡"),
    _("抛物面充电"),
    _("火焰弹"),
    _("种子闪光"),
    _("冲岩"),
    _("掷锚"),
    _("极光幕"),
    _("碉堡"),
    _("鸟嘴加农炮"),
    _("狂舞挥打"),
    _("燃尽"),
    _("鳞片噪音"),
    _("DD金勾臂"),
    _("龙锤"),
    _("火焰鞭"),
    _("迎头一击"),
    _("花朵加农炮"),
    _("花疗"),
    _("辅助齿轮"),
    _("十万马力"),
    _("冰锤"),
    _("磨砺"),
    _("树叶"),
    _("水流裂破"),
    _("猛扑"),
    _("暗影之光"),
    _("多属性攻击"),
    _("自然之怒"),
    _("花粉团"),
    _("嚣张"),
    _("棱镜镭射"),
    _("精神之牙"),
    _("净化"),
    _("觉醒之舞"),
    _("暗影之骨"),
    _("陷阱甲壳"),
    _("集沙"),
    _("修长之角"),
    _("日光刃"),
    _("泡影的咏叹调"),
    _("暗影偷盗"),
    _("速度互换"),
    _("缝影"),
    _("聚光灯"),
    _("跺脚"),
    _("吸取力量"),
    _("流星闪冲"),
    _("泪眼汪汪"),
    _("地狱突刺"),
    _("毒丝"),
    _("热带踢"),
    _("麻麻刺刺"),
    _("电气场地"),
    _("青草场地"),
    _("薄雾场地"),
    _("精神场地"),
    _("草之誓约"),
    _("火之誓约"),
    _("水之誓约"),
    _("戏法空间"),
    _("魔法空间"),
    _("奇妙空间"),
    _("戏法防守"),
    _("掀榻榻米"),
    _("快速防守"),
    _("广域防守"),
    _("水流环"),
    _("芳香薄雾"),
    _("打嗝"),
    _("传递礼物"),
    _("投掷"),
    _("飞身重压"),
    _("重力"),
    _("防守平分"),
    _("回复封锁"),
    _("等离子浴"),
    _("幸运咒语"),
    _("电磁飘浮"),
    _("磁场操控"),
    _("粉尘"),
    _("力量平分"),
    _("镜面属性"),
    _("单纯光束"),
    _("浸水"),
    _("意念移物"),
    _("万圣夜"),
    _("毒液陷阱"),
    _("森林诅咒"),
    _("惊爆大头"),
    _("光子喷涌"),
    _("等离子闪电拳"),
    _("输电"),
    _("查封"),
    _("妖精之锁"),
    _("破灭之光"),
    _("王牌"),
    _("您先请"),
    _("号令"),
    _("延后"),
    _("欢乐时光"),
    _("钢拳双击"),
    _("珍藏"),
    _("自由落体"),
    _("庆祝"),
    _("牵手"),
    _("极巨炮"),
    _("狙击"),
    _("紧咬不放"),
    _("大快朵颐"),
    _("背水一战"),
    _("沥青射击"),
    _("魔法粉"),
    _("龙箭"),
    _("茶会"),
    _("蛸固"),
    _("电喙"),
    _("鳃咬"),
    _("换场"),
    _("魂舞烈音爆"),
    _("扑击"),
    _("装饰"),
    _("鼓击"),
    _("捕兽夹"),
    _("火焰球"),
    _("巨兽斩"),
    _("巨兽弹"),
    _("气场轮"),
    _("广域破坏"),
    _("木枝突刺"),
    _("破音"),
    _("苹果酸"),
    _("万有引力"),
    _("灵魂冲击"),
    _("神奇蒸汽"),
    _("生命水滴"),
    _("拦堵"),
    _("假跪真撞"),
    _("流星突击"),
    _("无极光束"),
    _("铁蹄光线"),
    _("广域战力"),
    _("铁滚轮"),
    _("鳞射"),
    _("流星光束"),
    _("臂贝武器"),
    _("薄雾炸裂"),
    _("青草滑梯"),
    _("电力上升"),
    _("大地波动"),
    _("爬击"),
    _("妒火"),
    _("泄愤"),
    _("灵骚"),
    _("腐蚀气体"),
    _("指导"),
    _("快速折返"),
    _("三旋击"),
    _("双翼"),
    _("热沙大地"),
    _("丛林治疗"),
    _("暗冥强击"),
    _("水流连打"),
    _("冰冷视线"),
    _("雷鸣蹴击"),
    _("怒火中烧"),
    _("雷电囚笼"),
    _("巨龙威能"),
    _("Z-Move 1"),
    _("Z-Move 2"),
    _("Z-Move 3"),
    _("Z-Move 4"),
    _("Z-Move 5"),
    _("Z-Move 6"),
    _("Z-Move 7"),
    _("Z-Move 8"),
    _("Z-Move 9"),
    _("Z-Move 10"),
    _("Z-Move 11"),
    _("Z-Move 12"),
    _("Z-Move 13"),
    _("Z-Move 14"),
    _("Z-Move 15"),
    _("Z-Move 16"),
    _("Z-Move 17"),
    _("Z-Move 18"),
    _("Z-Move 19"),
    _("Z-Move 20"),
    _("Z-Move 21"),
    _("Z-Move 22"),
    _("Z-Move 23"),
    _("Z-Move 24"),
    _("Z-Move 25"),
    _("Z-Move 26"),
    _("Z-Move 27"),
    _("Z-Move 28"),
    _("Z-Move 29"),
    _("Z-Move 30"),
    _("Z-Move 31"),
    _("Z-Move 32"),
    _("Z-Move 33"),
    _("Z-Move 34"),
    _("Z-Move 35"),
    _("Z-Move 36"),
    _("Z-Move 37"),
    _("Z-Move 38"),
    _("Z-Move 39"),
    _("Z-Move 40"),
    _("Z-Move 41"),
    _("Z-Move 42"),
    _("Z-Move 43"),
    _("Z-Move 44"),
    _("Z-Move 45"),
    _("Z-Move 46"),
    _("Z-Move 47"),
    _("Z-Move 48"),
    _("Z-Move 49"),
    _("Z-Move 50"),
    _("Z-Move 51"),
    _("Z-Move 52"),
    _("Z-Move 53"),
    _("防壁"),
    _("攻击"),
    _("攻击"),
    _("拳斗"),
    _("拳斗"),
    _("飞冲"),
    _("飞冲"),
    _("酸毒"),
    _("酸毒"),
    _("大地"),
    _("大地"),
    _("岩石"),
    _("岩石"),
    _("虫蛊"),
    _("虫蛊"),
    _("幽魂"),
    _("幽魂"),
    _("钢铁"),
    _("钢铁"),
    _("火爆"),
    _("火爆"),
    _("水流"),
    _("水流"),
    _("草原"),
    _("草原"),
    _("闪电"),
    _("闪电"),
    _("超能"),
    _("超能"),
    _("寒冰"),
    _("寒冰"),
    _("龙骑"),
    _("龙骑"),
    _("恶霸"),
    _("恶霸"),
    _("妖精"),
    _("妖精"),
    _("灰飞鞭灭"),
    _("灰飞鞭灭"),
    _("地狱灭焰"),
    _("地狱灭焰"),
    _("水炮轰灭"),
    _("水炮轰灭"),
    _("蝶影蛊惑"),
    _("蝶影蛊惑"),
    _("万雷轰顶"),
    _("万雷轰顶"),
    _("特大金币"),
    _("特大金币"),
    _("会心一击"),
    _("会心一击"),
    _("幻影幽魂"),
    _("幻影幽魂"),
    _("激漩泡涡"),
    _("激漩泡涡"),
    _("极光旋律"),
    _("极光旋律"),
    _("热情拥抱"),
    _("热情拥抱"),
    _("资源再生"),
    _("资源再生"),
    _("臭气冲天"),
    _("臭气冲天"),
    _("液金熔击"),
    _("液金熔击"),
    _("狂擂乱打"),
    _("狂擂乱打"),
    _("破阵火球"),
    _("破阵火球"),
    _("狙击神射"),
    _("狙击神射"),
    _("旋风袭卷"),
    _("旋风袭卷"),
    _("天道七星"),
    _("天道七星"),
    _("岩阵以待"),
    _("岩阵以待"),
    _("炎石喷发"),
    _("炎石喷发"),
    _("酸不溜丢"),
    _("酸不溜丢"),
    _("琼浆玉液"),
    _("琼浆玉液"),
    _("沙尘漫天"),
    _("沙尘漫天"),
    _("异毒电场"),
    _("异毒电场"),
    _("百火焚野"),
    _("百火焚野"),
    _("天谴雷诛"),
    _("天谴雷诛"),
    _("睡魔降临"),
    _("睡魔降临"),
    _("幸福圆满"),
    _("幸福圆满"),
    _("钢铁阵法"),
    _("钢铁阵法"),
    _("劣化衰变"),
    _("劣化衰变"),
    _("夺命一击"),
    _("夺命一击"),
    _("夺命水流"),
    _("夺命水流"),
};