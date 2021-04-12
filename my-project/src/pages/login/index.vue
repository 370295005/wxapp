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
        <div class="waring" v-if="UserNameWaring && !LoginPage">
          {{ waring }}
        </div>
        <van-field
          placeholder="请输入密码"
          type="password"
          :value="inputPassword"
          @change="onPasswordChange"
        />
        <div class="waring" v-if="passwordWaring && !LoginPage">
          {{ waring }}
        </div>
        <van-field
          v-if="!LoginPage"
          placeholder="请输入手机号"
          type="number"
          :value="inputPhoneNumber"
          @change="onPhoneNumberChange"
        ></van-field>
        <div class="waring" v-if="phoneNumberWaring && !LoginPage">
          {{ waring }}
        </div>
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
      <van-dialog id="van-dialog" >
      </van-dialog>
    </div>
  </div>
</template>
<script>
import Toast from "@vant/weapp/dist/toast/toast";
import Dialog from "@vant/weapp/dist/dialog/dialog";
export default {
  data() {
    return {
      LoginPage: true,
      inputUserName: "",
      inputPassword: "",
      inputPhoneNumber: "",
      UserNameWaring: false,
      passwordWaring: false,
      phoneNumberWaring: false,
      waring: "",
    };
  },
  methods: {
    //监听用户名输入
    onUserNameChange(e) {
      this.inputUserName = e.mp.detail;
      // console.log(this.inputUserName);
      if (!this.LoginPage) {
        const regx = /^[a-zA-Z0-9]{6,18}$/;
        if (
          !regx.test(this.inputUserName) &&
          this.inputUserName.trim() !== ""
        ) {
          this.UserNameWaring = true;
          this.waring = "用户名格式有误,6-18位，字母或数字";
        } else if (this.inputUserName.trim() == "") {
          this.UserNameWaring = true;
          this.waring = "用户名不得为空";
        } else {
          this.waring = "";
          this.UserNameWaring = false;
        }
      }
    },
    //监听密码输入
    onPasswordChange(e) {
      this.inputPassword = e.mp.detail;
      // console.log(this.inputPassword);
      if (!this.LoginPage) {
        const regx = /^[0-9a-zA-Z]{6,18}$/;
        if (
          !regx.test(this.inputPassword) &&
          this.inputPassword.trim() !== ""
        ) {
          this.passwordWaring = true;
          this.waring = "密码格式有误,6-18位，字母或数字";
        } else if (this.inputPassword.trim() == "") {
          this.passwordWaring = true;
          this.waring = "密码不得为空";
        } else {
          this.waring = "";
          this.passwordWaring = false;
        }
      }
    },
    //监听手机号输入
    onPhoneNumberChange(e) {
      this.inputPhoneNumber = e.mp.detail;
      // console.log(this.inputPhoneNumber);
      if (!this.LoginPage) {
        const regx = /^1[3-9]\d{9}/;
        if (
          !regx.test(this.inputPhoneNumber) &&
          this.inputPhoneNumber.trim() !== ""
        ) {
          this.phoneNumberWaring = true;
          this.waring = "手机号格式有误";
        } else if (this.inputPhoneNumber.trim() == "") {
          this.waring = "手机号不得为空";
        } else {
          this.waring = "";
          this.phoneNumberWaring = false;
        }
      }
    },
    //登录(注册)
    login(e) {
      if (this.waring) {
        Toast.fail(this.waring);
      } else {
        //登录
        if (this.LoginPage) {
          if (
            this.inputUserName.trim() !== "" &&
            this.inputPassword.trim() !== ""
          ) {
            Toast.loading({
              duration: 0, //持续展示Toast
              forbidClick: true,
              message: this.LoginPage ? "登录中" : "注册中",
              loadingType: "spinner",
            });
            const that = this;
            wx.request({
              url: `http://203.195.212.95/login.php`,
              method: "POST",
              data: {
                username: this.inputUserName,
                password: this.inputPassword,
              },
              header: {
                "content-Type": "application/x-www-form-urlencoded",
              },
              success(res) {
                if (res.data.length === 0) {
                  setTimeout(() => {
                    Toast.fail("用户名或密码错误");
                    that.inputPassword = "";
                  }, 500);
                }
              },
            });
          } else {
            Toast.fail("账号或密码为空");
          }
        } else {
          //注册
          if (
            this.inputUserName.trim() == !"" &&
            this.inputPassword.trim() == !"" &&
            this.inputPhoneNumber.trim() == !""
          ) {
            Toast.loading({
              duration: 0, //持续展示Toast
              forbidClick: true,
              message: this.LoginPage ? "登录中" : "注册中",
              loadingType: "spinner",
            });
            const that = this;
            wx.request({
              url: "http://203.195.212.95/register.php",
              method: "POST",
              data: {
                username: this.inputUserName,
                password: this.inputPassword,
                phonenumber: this.inputPhoneNumber,
              },
              header: {
                "content-Type": "application/x-www-form-urlencoded",
              },
              success(res) {
                if (res.data.status == 1) {
                  setTimeout(() => {
                    Toast.success("注册成功");
                    that.LoginPage = true;
                  }, 500);
                } else {
                  setTimeout(() => {
                    Toast.fail("注册失败");
                    that.inputPassword = "";
                    that.inputUserName = "";
                    that.inputPhoneNumber = "";
                  }, 500);
                }
              },
            });
          } else {
            Toast.fail("请正确输入注册信息");
          }
        }
      }
    },
    //切换登录注册
    register(e) {
      this.inputUserName = "";
      this.inputPassword = "";
      this.LoginPage = !this.LoginPage;
    },
    //忘记密码
    forgetPassword(e) {
      console.log("wangjimima");
      Dialog.confirm({
        title: "忘记密码",
        message: "弹窗内容",
      })
        .then(() => {
          // on confirm
          console.log("confirm");

        })
        .catch(() => {
          // on cancel
          console.log("cancel");
        });
    },
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
    margin-bottom: 20px;
    .waring {
      padding: 10px 15px;
      color: red;
      font-size: 12px;
    }
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