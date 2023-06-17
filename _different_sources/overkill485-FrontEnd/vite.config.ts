// import legacy from "@vitejs/plugin-legacy";

import { visualizer } from 'rollup-plugin-visualizer';
import path from "path";
import {
    defineConfig
} from "vite";

import { ViteFaviconsPlugin } from 'vite-plugin-favicon2';
import type { UserConfig } from "vite";


// export default defineConfig({
const config: UserConfig = {
  base: "",
  root: "src/",
  assetsInclude: ["**/*.svg"],
  server: {
    port: 5555,
    open: true,
    // proxy: {
    //     '/ws': {
    //         target: 'http://jsonplaceholder.typicode.com',
    //         changeOrigin: true,
    //         rewrite: (path) => path.replace(/^\/ws/, ''),
    //         // target: 'ws://bmsesp.local/ws',
    //         // changeOrigin: true,
    //         // ws: true,
    //     },
    // }
  },
  preview: {
    port: 7777,
  },
  plugins: [
    visualizer({
      open: true,
      brotliSize: true,
      gzipSize: true,
      // sourcemap:true,
      template: "sunburst",
    }),
    ViteFaviconsPlugin({
      /** Your source logo (Will default to ) */
      logo: "./src/pwa-x512.png",
      inject: false,
      favicons: {
        appName: "BMSESP",
        appDescription: "BMSESP - the best webApplication!!!",
        developerName: "p3xx",
        developerURL: null, // prevent retrieving from the nearest package.json
        background: "#ddd",
        theme_color: "#333",
        display: "standalone", // Preferred display mode: "fullscreen", "standalone", "minimal-ui" or "browser". `string`
        orientation: "any",
        appleStatusBarStyle: "black-translucent",
        start_url: "/",
        scope: "/",
        icons: {
          // android: {
          //     mask: true,
          //     overlayGlow: true,
          //     overlayShadow: true,
          // },
          android: false,
          appleIcon: false,
          appleStartup: false,
          coast: false,
          favicons: true,
          firefox: false,
          windows: false,
          yandex: false,
        },
      },
    }),
    //* ViteFaviconsPlugin("./img/android-chrome-512x512.png"),

    /* legacy({
           targets: ['ie >= 11'],
           additionalLegacyPolyfills: ['regenerator-runtime/runtime']
           
           //////////////////
           // targets: ['defaults', 'not IE 11'],
         }),*/
  ],
  build: {
    // include: 'js/**',
    // outDir: path.join(__dirname, "distr"),

    emptyOutDir: true,
    // cssCodeSplit: true,
    copyPublicDir: true,
    cssMinify: true,
    assetsDir: "someshit",
    reportCompressedSize: true,
    // sourcemap: true,
    // assetsInlineLimit: 1000,
    outDir: "../distrib",
    sourcemap: false,
    manifest: true,
    rollupOptions: {},
    minify: "terser",
    commonjsOptions: {
      include: ["src/js/*"],
      // exclude: ["ckeditor/*"],
    },
    chunkSizeWarningLimit: 5000,
  },
};


export default config;