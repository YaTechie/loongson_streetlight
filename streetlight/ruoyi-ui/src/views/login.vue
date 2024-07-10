<template>
  <div class="content">
    <div class="login-wrapper">
      <div class="left-img">
        <!-- <div class="glass"> -->
          <!-- <div class="tips">
            <div class="title">LOONGSON 龙芯</div> -->
            <!-- <h1>Explore The Universe</h1>
            <span>5 Million+ people have joined our network.</span>
            <span>we invite you to join the tribe</span> -->
          <!-- </div>
        </div> -->
      </div>
      <div class="right-login-form">
        <div class="form-wrapper">
          <h1>Log in</h1>
          <el-form ref="loginForm" :model="loginForm" :rules="loginRules" class="login-form">
            <div class="input-items">
              <span class="input-tips">
                Email Address
              </span>
              <input type="text" class="inputs" placeholder="Enter your email" v-model="loginForm.username">
            </div>
            <!-- <el-form-item prop="username">
              <el-input
                v-model="loginForm.username"
                type="text"
                auto-complete="off"
                placeholder="Enter your email"
              >
                <svg-icon slot="prefix" icon-class="user" class="el-input__icon input-icon" />
              </el-input>
            </el-form-item> -->
            <div class="input-items">
              <span class="input-tips">
                Password
              </span>
              <input type="password" class="inputs" placeholder="Enter password" v-model="loginForm.password">
              <!-- <span class="forgot">Forgot Password</span> -->
            </div>
            <!-- <el-form-item prop="password">
              <el-input
                v-model="loginForm.password"
                type="password"
                auto-complete="off"
                placeholder="Enter password"
                @keyup.enter.native="handleLogin"
              >
                <svg-icon slot="prefix" icon-class="password" class="el-input__icon input-icon" />
              </el-input>
              <span class="forgot">Forgot Password</span>
            </el-form-item> -->

            <el-form-item prop="code" v-if="captchaEnabled">
              <el-input
                v-model="loginForm.code"
                auto-complete="off"
                placeholder="验证码"
                style="width: 63%"
                @keyup.enter.native="handleLogin"
              >
                <svg-icon slot="prefix" icon-class="validCode" class="el-input__icon input-icon" />
              </el-input>
              <div class="login-code">
                <img :src="codeUrl" @click="getCode" class="login-code-img"/>
              </div>
            </el-form-item>
            <el-checkbox v-model="loginForm.rememberMe" style="margin:0px 0px 25px 0px;">记住密码</el-checkbox>
            <el-form-item style="width:100%;">
              <el-button
              class="btn"
                :loading="loading"
                size="medium"
                type="primary"
                style="width:100%;"
                @click.native.prevent="handleLogin"
              >
                <span v-if="!loading">Log in</span>
                <span v-else>Log in...</span>
              </el-button>
              <div style="float: right;" v-if="register">
                <router-link class="link-type" :to="'/register'">立即注册</router-link>
              </div>
            </el-form-item>
          </el-form>
          <div class="siginup-tips">
            <span>Don't Have An Account?</span>
            <span>Signup</span>
          </div>
          <div class="other-login">
            <div class="divider">
              <span class="line"></span>
              <span class="divider-text">or</span>
              <span class="line"></span>
            </div>
            <div class="other-login-wrapper">
              <div class="other-login-item">
                <!-- <img src="@/views/asset/QQ.png" alt="QQ"> -->
              </div>
              <div class="other-login-item">
                <!-- <img src="@/views/asset/WeChat.png" alt="WeChat"> -->
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { getCodeImg } from "@/api/login";
import Cookies from "js-cookie";
import { encrypt, decrypt } from '@/utils/jsencrypt'

export default {
  name: "Login",
  data() {
    return {
      codeUrl: "",
      loginForm: {
        username: "admin",
        password: "admin123",
        rememberMe: false,
        code: "",
        uuid: ""
      },
      loginRules: {
        username: [
          { required: true, trigger: "blur", message: "请输入您的账号" }
        ],
        password: [
          { required: true, trigger: "blur", message: "请输入您的密码" }
        ],
        code: [{ required: true, trigger: "change", message: "请输入验证码" }]
      },
      loading: false,
      // 验证码开关
      captchaEnabled: true,
      // 注册开关
      register: false,
      redirect: undefined
    };
  },
  watch: {
    $route: {
      handler: function(route) {
        this.redirect = route.query && route.query.redirect;
      },
      immediate: true
    }
  },
  created() {
    this.getCode();
    this.getCookie();
  },
  methods: {
    getCode() {
      getCodeImg().then(res => {
        this.captchaEnabled = res.captchaEnabled === undefined ? true : res.captchaEnabled;
        if (this.captchaEnabled) {
          this.codeUrl = "data:image/gif;base64," + res.img;
          this.loginForm.uuid = res.uuid;
        }
      });
    },
    getCookie() {
      const username = Cookies.get("username");
      const password = Cookies.get("password");
      const rememberMe = Cookies.get('rememberMe')
      this.loginForm = {
        username: username === undefined ? this.loginForm.username : username,
        password: password === undefined ? this.loginForm.password : decrypt(password),
        rememberMe: rememberMe === undefined ? false : Boolean(rememberMe)
      };
    },
    handleLogin() {
      this.$refs.loginForm.validate(valid => {
        if (valid) {
          this.loading = true;
          if (this.loginForm.rememberMe) {
            Cookies.set("username", this.loginForm.username, { expires: 30 });
            Cookies.set("password", encrypt(this.loginForm.password), { expires: 30 });
            Cookies.set('rememberMe', this.loginForm.rememberMe, { expires: 30 });
          } else {
            Cookies.remove("username");
            Cookies.remove("password");
            Cookies.remove('rememberMe');
          }
          this.$store.dispatch("Login", this.loginForm).then(() => {
            this.$router.push({ path: this.redirect || "/" }).catch(()=>{});
          }).catch(() => {
            this.loading = false;
            if (this.captchaEnabled) {
              this.getCode();
            }
          });
        }
      });
    }
  }
};
</script>

<style scoped>
* {
  padding: 0;
  margin: 0;
  font-family: "Aguazyuan", Courier, monospace;
}

.content {
  /* display: flex;
  justify-content: center;
  align-items: center;
  height: 100%;
  background-image: url("../assets/images/login-background.jpg");
  background-size: cover; */
  width: 100vw;
  height: 100vh;
  background-color: rgb(186, 175, 221);
  /* background  : url("../assets/images/login-background.jpg") no-repeat; */
  position: relative;
}
.content .login-wrapper {
  width: 70vw;
  height: 80vh;
  background-color: #fff;
  border-radius: 40px;
  position: absolute;
  left: 50%;
  top: 50%;
  transform: translate(-50%, -50%);
  display: flex;
}
.content .login-wrapper .left-img {
  border-radius: 40px;
  flex: 1;
  background: url(../assets/images/login-background.jpg) no-repeat;
  
  background-size: 116% 100%;
  position: relative;
}
.content .login-wrapper .left-img .glass {
  width: 60%;
  padding: 20px;
  color: #fff;
  position: absolute;
  top: 20%;
  left: 50%;
  transform: translate(-50%, -20%);
  background: rgba(255, 255, 255, 0.1);
  box-shadow: 0 8px 32px 0 rgba(31, 38, 135, 0.37);
  backdrop-filter: blur(10px);
  -webkit-backdrop-filter: blur(10px);
  border-radius: 15px;
  border: 1px solid rgba(255, 255, 255, 0.48);
}
.content .login-wrapper .left-img .glass .tips .title {
  width: 25%;
  font-weight: 600;
  font-size: 15px;
  text-align: center;
  padding: 10px;
  overflow: hidden;
  background: rgba(255, 255, 255, 0.1);
  box-shadow: 0 8px 32px 0 rgba(31, 38, 135, 0.37);
  backdrop-filter: blur(10px);
  -webkit-backdrop-filter: blur(10px);
  border-radius: 15px;
  border: 1px solid rgba(255, 255, 255, 0.48);
}
.content .login-wrapper .left-img .glass .tips h1 {
  margin: 15px 0;
}
.content .login-wrapper .left-img .glass .tips span {
  margin: 5px 0;
  display: block;
  font-weight: 100;
}
.content .login-wrapper .right-login-form {
  flex: 1;
  position: relative;
}
.content .login-wrapper .right-login-form .form-wrapper {
  position: absolute;
  left: 50%;
  top: 50%;
  transform: translate(-50%, -50%);
}
.content .login-wrapper .right-login-form .form-wrapper h1 {
  font-size: 30px;
  text-align: center;
}
.content .login-wrapper .right-login-form .form-wrapper .input-items {
  margin: 20px 0;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .input-items
  .input-tips {
  display: inline-block;
  font-weight: 600;
  font-size: 20px;
  margin: 10px 0;
}
.content .login-wrapper .right-login-form .form-wrapper .input-items .inputs {
  width: 100%;
  height: 50px;
  border-radius: 10px;
  border: 1px solid rgb(227, 227, 227);
  padding: 20px;
  box-sizing: border-box;
  outline: none;
  transition: 0.5s;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .input-items
  .inputs:focus {
  border: 1px solid rgb(128, 106, 196);
}
.content .login-wrapper .right-login-form .form-wrapper .input-items .forgot {
  float: right;
  font-weight: 600;
  text-align: right;
  margin: 5px 0;
  font-size: 13px;
  cursor: pointer;
}
.content .login-wrapper .right-login-form .form-wrapper .btn {
  width: 100%;
  height: 50px;
  background-color: rgb(128, 106, 196);
  border: 0;
  border-radius: 10px;
  color: #fff;
  font-size: 25px;
  margin: 20px 0;
  cursor: pointer;
}
.content .login-wrapper .right-login-form .form-wrapper .siginup-tips {
  text-align: center;
  font-weight: 600;
  margin: 10px 0;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .siginup-tips
  span:last-child {
  color: rgb(110, 87, 179);
  cursor: pointer;
}
.content .login-wrapper .right-login-form .form-wrapper .other-login .divider {
  width: 100%;
  margin: 20px 0;
  text-align: center;
  display: flex;
  align-items: center;
  justify-content: space-between;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .other-login
  .divider
  .line {
  display: inline-block;
  max-width: 35%;
  width: 35%;
  flex: 1;
  height: 1px;
  background-color: rgb(162, 172, 185);
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .other-login
  .divider
  .divider-text {
  vertical-align: middle;
  margin: 0 20px;
  display: inline-block;
  width: 50px;
  color: rgb(162, 172, 185);
  white-space: normal;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .other-login
  .other-login-wrapper {
  width: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .other-login
  .other-login-wrapper
  .other-login-item {
  width: 70px;
  padding: 10px;
  text-align: center;
  border-radius: 10px;
  cursor: pointer;
  font-weight: 600;
  color: rgb(51, 49, 116);
  margin: 0 10px;
  transition: 0.4s;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .other-login
  .other-login-wrapper
  .other-login-item
  img {
  width: 40px;
  height: 40px;
  vertical-align: middle;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .other-login
  .other-login-wrapper
  .other-login-item
  span {
  vertical-align: middle;
}
.content
  .login-wrapper
  .right-login-form
  .form-wrapper
  .other-login
  .other-login-wrapper
  .other-login-item:hover {
  width: 80px;
  height: 50%;
  background-color: #fff;
  border: 0;
  box-shadow: 1px 10px 32px 1px rgba(186, 175, 221, 0.37);
}

@media (max-width: 1024px) {
  .login-wrapper {
    padding: 20px;
  }
  .login-wrapper .left-img {
    display: none;
  }
  .login-wrapper .right-login-form {
    padding: 20px;
  }
}
</style>
