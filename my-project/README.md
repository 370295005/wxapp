# my-project

> 毕业设计

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

## QA

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
mqtt模块新版本有问题需将至3.0.0 connect引入路径有问题 4.1.0版本可以使用
npm install mqtt@3.0.0 --save
import { connect } from "mqtt/dist/mqtt.js";
import { connect } from "mqtt/dist/mqtt.min.js";
5. 接收到的订阅消息是十六进制的
使用JSON.parse()转换成字符串
6. 微信小程序npm构建失败 SyntaxError:Invalid number(108:10)
疑似是node-sass引起的问题
7. 无法引入vant weapp 提示组件未找到
修改webpack打包规则 使其能正确找到组件位置
8. 安装mqtt3.0.0后构建npm会报错，会提示部分入口文件未找到
不需要构建npm 且mqtt版本使用4.1.0
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
服务端使用nginx作为服务器，并反向代理8084端口。
17. PHP中文返回值会乱码
18. 部分情况下会收到两条一模一样的数据
```

### 16. 安卓真机调试无法连接至mqtt服务器

配置反向代理使443端口在遇到/mqtt的请求时将其转发至8084端口

```
location /mqtt {
      proxy_pass https://127.0.0.1:8084/mqtt;
      proxy_set_header Host $host;
      proxy_set_header X-Real-IP $remote_addr;
      proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
      # client_max_body_size 35m;
      proxy_http_version 1.1;
      proxy_set_header Upgrade $http_upgrade;
      proxy_set_header Connection "upgrade";    
    }
location  ~ \.php$ {
      root           html;
      fastcgi_pass   127.0.0.1:9000;
      fastcgi_index  index.php;
      fastcgi_param  SCRIPT_FILENAME $document_root$fastcgi_script_name;
      include        fastcgi_params;
    }
```

