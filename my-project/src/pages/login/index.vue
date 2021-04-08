<template>
  <div>
    <div class="header" v-if="LoginPage">
      <div class="header-title">请登录</div>
      <div class="header-info">Please Login</div>
    </div>
    <div class="header" v-else>
      <div class="header-title">请注册</div>
      <div class="header-info">Please Register</div>
    </div>
    <div class="body">
      <div class="login-form">
        <van-field
          placeholder="请输入用户名"
          :value="inputUserName"
          @change="onUserNameChange"
        />
        <van-field
          placeholder="请输入密码"
          type="password"
          :value="inputUserPassword"
          @change="onPasswordChange"
        />
        <van-field
          v-if="!LoginPage"
          placeholder="请输入手机号"
          :value="inputPhoneNumber"
          @change="onPhoneNumberChange"
        ></van-field>
      </div>
      <van-button
        slot="button"
        type="primary"
        round
        block
        color="#3d7df9"
        @click="login"
        >{{ LoginPage ? "登录" : "注册" }}</van-button
      >
      <div class="other-option">
        <span @click="register">{{ LoginPage ? "注册账号" : "登录账号" }}</span>
        <span style="margin: 0px 30px">|</span>
        <span @click="forgetPassword">忘记密码</span>
      </div>
      <van-toast id="van-toast" />
    </div>
  </div>
</template>
<script>
import Toast from "vant-weapp/dist/toast/toast";
export default {
  data() {
    return {
      LoginPage: true,
      inputUserName: "",
      inputUserPassword: "",
      inputPhoneNumber: "",
    };
  },
  methods: {
    //监听用户名输入
    onUserNameChange(e) {
      this.inputUserName = e.mp.detail;
    },
    //监听密码输入
    onPasswordChange(e) {
      this.inputPassword = e.mp.detail;
    },
    //监听手机号输入
    onPhoneNumberChange() {
      this.iinputPhoneNumber = e.mp.detail;
    },
    //登录
    login(e) {
      console.log(e);
      Toast.loading({
        duration: 2000, //持续展示Toast
        forbidClick: true,
        message: this.LoginPage ? "登录中" : "注册中",
      });
      const that = this
      // wx.request({
      //   url:'www.nash141.cloud',
      //   data:{
      //     username:that.UserName,
      //     password:that.password,
      //   },
      //   header:{
      //     'content-type':'application/json'
      //   },
      //   success:res=>{
      //     console.log(res);
      //   }
      // })
    },
    //注册函数
    register(e) {
      console.log(e);
      this.LoginPage = !this.LoginPage;
    },
    //忘记密码
    forgetPassword(e) {},
  },
};
</script>


<style lang="scss" scoped>
.header {
  height: 100px;
  padding: 25px 0px;
  background-color: #3d7ef9;
  color: white;
  padding-left: 20px;
  .header-title {
    font-size: 28px;
    font-weight: 500;
  }
  .header-info {
    font-size: 14px;
  }
}

.body {
  border-radius: 15px 15px 0px 0px;
  margin-top: -20px;
  background-color: #fff;
  padding: 20px;
  .login-form {
    margin-bottom: 30px;
  }
  .other-option {
    display: flex;
    justify-content: center;
    margin-top: 20px;
    color: #9f9f9f;
    font-size: 14px;
  }
}
</style>