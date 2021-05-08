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
        <div class="sex-radio" v-if="!LoginPage">
          <van-radio-group
            direction="horizontal"
            :value="sex"
            @change="onSelectSex"
          >
            <van-radio name="1">男</van-radio>
            <van-radio name="0">女</van-radio>
          </van-radio-group>
        </div>
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
      <van-dialog
        use-slot
        title="找回密码"
        id="van-dialog"
        transition="fade"
        :show="isforget"
        @confirm="forgetPassword"
        show-cancel-button
      >
        <van-field
          placeholder="请输入注册手机号"
          label="手机号"
          title-width="60px"
          @change="onPasswordForget"
          :value="inputForgetPassword"
          type="number"
        ></van-field>
      </van-dialog>
      <van-dialog
        use-slot
        title="重置密码"
        id="van-dialog"
        transition="fade"
        :show="isreset"
        show-cancel-button
        @confirm="resetPassword"
      >
        <van-field
          :placeholder="forgetusername"
          label="用户名"
          title-width="60px"
          disabled
        ></van-field>
        <van-field
          placeholder="请输入新密码"
          label="新密码"
          title-width="60px"
          @change="onNewPassword"
          type="password"
        ></van-field>
      </van-dialog>
    </div>
  </div>
</template>
<script>
import Toast from "@vant/weapp/dist/toast/toast";
export default {
  data() {
    return {
      LoginPage: true,
      inputUserName: "",
      inputPassword: "",
      inputPhoneNumber: "",
      inputForgetPhoneNumber: "", // 忘记密码输入的手机号
      inputNewPassword: "", // 忘记密码输入的新密码
      sex: "",
      forgetusername: "",
      UserNameWaring: false,
      passwordWaring: false,
      phoneNumberWaring: false,
      waring: "",
      isreset: false,
      isforget: false,
    };
  },
  methods: {
    // 监听用户名输入
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
          this.phoneNumberWaring = false;
          this.passwordWaring = false;
          this.waring = "用户名格式有误,6-18位，字母或数字下划线";
        } else if (this.inputUserName.trim() === "") {
          this.UserNameWaring = true;
          this.phoneNumberWaring = false;
          this.passwordWaring = false;
          this.waring = "用户名不得为空";
        } else {
          this.waring = "";
          this.UserNameWaring = false;
        }
      }
    },
    // 监听密码输入
    onPasswordChange(e) {
      this.inputPassword = e.mp.detail;
      // console.log(this.inputPassword);
      if (!this.LoginPage) {
        const regx = /^[0-9a-zA-Z@_?]{6,18}$/;
        if (
          !regx.test(this.inputPassword) &&
          this.inputPassword.trim() !== ""
        ) {
          this.passwordWaring = true;
          this.waring = "密码格式有误,6-18位，字母或数字下划线";
          this.UserNameWaring = false;
          this.phoneNumberWaring = false;
        } else if (this.inputPassword.trim() === "") {
          this.passwordWaring = true;
          this.waring = "密码不得为空";
          this.UserNameWaring = false;
          this.phoneNumberWaring = false;
        } else {
          this.waring = "";
          this.passwordWaring = false;
        }
      }
    },
    // 监听手机号输入
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
          this.passwordWaring = false;
          this.UserNameWaring = false;
          this.waring = "手机号格式有误";
        } else if (this.inputPhoneNumber.trim() === "") {
          this.phoneNumberWaring = true;
          this.waring = "手机号不得为空";
          this.passwordWaring = false;
          this.UserNameWaring = false;
        } else {
          this.waring = "";
          this.phoneNumberWaring = false;
        }
      }
    },
    // 监听性别选择
    onSelectSex(e) {
      console.log(e.mp.detail);
      this.sex = e.mp.detail;
      console.log(this.sex);
    },
    // 监听忘记密码时输入的手机号
    onPasswordForget(e) {
      this.inputForgetPhoneNumber = e.mp.detail;
      // console.log(this.inputForgetPhoneNumber);
    },
    // 监听新输入的密码
    onNewPassword(e) {
      this.inputNewPassword = e.mp.detail;
    },
    // 登录(注册)
    login(e) {
      if (this.waring) {
        Toast.fail(this.waring);
      } else {
        // 登录
        if (this.LoginPage) {
          if (
            this.inputUserName.trim() !== "" &&
            this.inputPassword.trim() !== ""
          ) {
            Toast.loading({
              duration: 0, // 持续展示Toast
              forbidClick: true,
              message: this.LoginPage ? "登录中" : "注册中",
              loadingType: "spinner",
            });
            const that = this;
            wx.request({
              url: `http://www.nash141.cloud/mysql/login.php`,
              method: "POST",
              data: {
                username: this.inputUserName,
                password: this.inputPassword,
              },
              header: {
                "content-Type": "application/x-www-form-urlencoded",
              },
              success(res) {
                console.log(res);
                if (res.data.res === 0) {
                  setTimeout(() => {
                    Toast.fail("用户名或密码错误");
                    that.inputPassword = "";
                  }, 500);
                } else if (res.data.res === 1) {
                  Toast.success("登录成功");
                  that.$store.commit("SetCurrentUser", res.data.userinfo);
                  that.$store.commit("SetCurrentDevice", res.data.deviceinfo);
                  if (res.data.deviceinfo) {
                    setTimeout(() => {
                      wx.switchTab({
                        url: "/pages/index/main",
                      });
                    }, 500);
                  } else {
                    setTimeout(() => {
                      wx.navigateTo({
                        url: "/pages/relation/main",
                      });
                    }, 500);
                  }
                }
              },
            });
          } else {
            Toast.fail("账号或密码为空");
          }
        } else {
          // 注册
          if (
            this.inputUserName.trim() !== "" &&
            this.inputPassword.trim() !== "" &&
            this.inputPhoneNumber.trim() !== "" &&
            this.sex !== ""
          ) {
            Toast.loading({
              duration: 0, // 持续展示Toast
              forbidClick: true,
              message: this.LoginPage ? "登录中" : "注册中",
              loadingType: "spinner",
            });
            const that = this;
            wx.request({
              url: "http://www.nash141.cloud/mysql/register.php",
              method: "POST",
              data: {
                username: this.inputUserName,
                password: this.inputPassword,
                phonenumber: this.inputPhoneNumber,
                sex: this.sex,
                admin: 0,
                status: 1,
              },
              header: {
                "content-Type": "application/x-www-form-urlencoded",
              },
              success(res) {
                if (res.data.res === 1) {
                  setTimeout(() => {
                    Toast.success("注册成功");
                    that.LoginPage = true;
                  }, 500);
                } else {
                  setTimeout(() => {
                    console.log(res);
                    Toast.fail(res.data);
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
    // 切换登录注册
    register(e) {
      this.inputUserName = "";
      this.inputPassword = "";
      this.inputPhoneNumber = "";
      this.LoginPage = !this.LoginPage;
    },
    // 忘记密码
    forgetPassword(e) {
      const that = this;
      this.isforget = !this.isforget;
      Toast.loading({
        message: "验证中...",
        duration: 0,
        loadingType: "circular",
        transition: "van-fade",
        overlay: true,
      });
      if (this.inputForgetPhoneNumber.trim() !== "") {
        wx.request({
          url: "http://www.nash141.cloud/mysql/findpassword.php",
          methods: "POST",
          data: {
            phonenumber: this.inputForgetPhoneNumber,
          },
          success(res) {
            setTimeout(() => {
              Toast.clear();
              that.isreset = !that.isreset;
            }, 500);
            if (res.data.length !== 0) {
              that.forgetusername = res.data[0].username;
            } else {
              Toast.fail("该手机号未注册");
            }
            that.inputForgetPassword = "";
          },
        });
      }
    },
    // 重置密码
    resetPassword(e) {
      const that = this;
      wx.request({
        url: `http://www.nash141.cloud/mysql/reset.php`,
        methods: "POST",
        data: {
          password: this.inputNewPassword,
          phonenumber: this.inputForgetPhoneNumber,
        },
        success(res) {
          that.inputNewPassword = "";
          that.inputForgetPhoneNumber = "";
          Toast.success(res.data.message);
        },
      });
    },
  },
};
</script>


<style lang="scss">
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
    margin: 40px 0px;
    .waring {
      padding: 10px 15px;
      color: red;
      font-size: 12px;
    }
    .sex-radio {
      padding: 10px 16px;
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
.van-toast--icon {
  width: unset !important;
}
</style>