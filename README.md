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
8. 安装mqtt3.0.0后构建npm会报错
9. vue不提供onblur事件
10. echarts动态渲染数据
https://www.cnblogs.com/wangyang0210/p/10683139.html
```
