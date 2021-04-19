# my-project

> final design

## Build Setup

``` bash
# 初始化项目
vue init mpvue/mpvue-quickstart myproject
cd myproject

# 安装依赖
yarn

# 开发时构建
npm dev

# 打包构建
npm build

# 指定平台的开发时构建(微信、百度、头条、支付宝)
npm dev:wx
npm dev:swan
npm dev:tt
npm dev:my

# 指定平台的打包构建
npm build:wx
npm build:swan
npm build:tt
npm build:my

# 生成 bundle 分析报告
npm run build --report
```

For detailed explanation on how things work, checkout the [guide](http://vuejs-templates.github.io/webpack/) and [docs for vue-loader](http://vuejs.github.io/vue-loader).

## error and solution

```
1. TypeError: Cannot read property 'call' of undefined
部分初始化后的文件没有删除干净，删除后正常编译
2. Node Sass version 5.0.0 is incompatible with^4.0.0
sass-loader的期望版本为4.x 而当前node-sass版本为5.0.0 
npm uninstall node-sass
npm install node-sass@4.14.1
3. TypeError: this.getOptions is not a function
sass-loader安装版本过高
npm uninstall sass-loader
npm install sass-loader@7.3.1
4. 无法连接至mqtt服务器
mqtt模块新版本有问题需将至3.0.0 connect引入路径有问题
npm install mqtt@3.0.0 --save
import { connect } from "mqtt/dist/mqtt.js";
5. 接收到的订阅消息是十六进制的
使用JSON.parse()转换成字符串
6. 微信小程序npm构建失败 SyntaxError:Invalid number(108:10)
疑似是node-sass引起的问题
7. 无法引入vant weapp 提示组件未找到
修改webpack打包规则 使其能正确找到组件位置
8. 安装mqtt3.0.0后构建npm会报错，会提示部分入口文件未找到
9. vue不提供onblur事件
10. echarts动态渲染数据
https://www.cnblogs.com/wangyang0210/p/10683139.html
11. echarts dataZoom不生效的问题
引入了错误的文件 echarts/dist/echarts.simple.min
应当为 echarts/dist/echarts.min
12. datazoom生效后数据不显示的问题
x轴data不得为空
13. vant提供的toast若提示信息过长会自动换行导致样式不好看的问题
把van-toast--icon 的宽度设置去除即可
14. 在还没有进入数据页面时，canvas还没有初始化，若此时传入数据则会报错
15. 传入动态数据时会重新初始化canvas导致闪屏
通过echarts.setOption()方法可以动态渲染数据，就不会闪屏
16. 安卓真机调试无法连接至mqtt服务器
安卓真机调试的问题，端口号会被当做把host中的一部分，即8080 和 443 端口外其他端口都不能访问
17. PHP中文返回值会乱码
```

