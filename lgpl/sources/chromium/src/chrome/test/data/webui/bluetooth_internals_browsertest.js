// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview Tests for chrome://bluetooth-internals
 */

/** @const {string} Path to source root. */
var ROOT_PATH = '../../../../';

/**
 * Test fixture for BluetoothInternals WebUI testing.
 * @constructor
 * @extends testing.Test
 */
function BluetoothInternalsTest() {
  this.adapterFactory = null;
  this.setupResolver = new PromiseResolver();
}

BluetoothInternalsTest.prototype = {
  __proto__: testing.Test.prototype,

  /** @override */
  browsePreload: 'chrome://bluetooth-internals',

  /** @override */
  isAsync: true,

  /** @override */
  runAccessibilityChecks: false,

  /** @override */
  extraLibraries: [
    ROOT_PATH + 'third_party/mocha/mocha.js',
    ROOT_PATH + 'chrome/test/data/webui/mocha_adapter.js',
    ROOT_PATH + 'ui/webui/resources/js/promise_resolver.js',
    ROOT_PATH + 'ui/webui/resources/js/cr.js',
    ROOT_PATH + 'ui/webui/resources/js/util.js',
    ROOT_PATH + 'chrome/test/data/webui/test_browser_proxy.js',
  ],

  preLoad: function() {
    // A function that is called from chrome://bluetooth-internals to allow this
    // test to replace the real Mojo browser proxy with a fake one, before any
    // other code runs.
    window.setupFn = function() {
      return importModules([
        'content/public/renderer/frame_interfaces',
        'device/bluetooth/public/interfaces/adapter.mojom',
        'device/bluetooth/public/interfaces/device.mojom',
        'mojo/public/js/bindings',
      ]).then(function([frameInterfaces, adapter, device, bindings]) {
        /**
         * A test adapter factory proxy for the chrome://bluetooth-internals
         * page.
         *
         * @constructor
         * @extends {TestBrowserProxy}
         */
        var TestAdapterFactoryProxy = function() {
          TestBrowserProxy.call(this, [
            'getAdapter',
          ]);

          this.binding = new bindings.Binding(adapter.AdapterFactory, this);
          this.adapter = new TestAdapterProxy();
          this.adapterBinding_ = new bindings.Binding(adapter.Adapter,
                                                      this.adapter);
        };

        TestAdapterFactoryProxy.prototype = {
          __proto__: TestBrowserProxy.prototype,
          getAdapter: function() {
            this.methodCalled('getAdapter');

            // Create message pipe bound to TestAdapter.
            return Promise.resolve({
              adapter: this.adapterBinding_.createInterfacePtrAndBind(),
            });
          }
        };

        /**
         * A test adapter proxy for the chrome://bluetooth-internals page.
         * @constructor
         * @extends {TestBrowserProxy}
         */
        var TestAdapterProxy = function() {
          TestBrowserProxy.call(this, [
            'getInfo',
            'getDevices',
            'setClient',
          ]);

          this.deviceProxyMap = new Map();
          this.adapterInfo_ = null;
          this.devices_ = [];
          this.connectResult_ = adapter.AdapterInfo.SUCCESS;
        };

        TestAdapterProxy.prototype = {
          __proto__: TestBrowserProxy.prototype,

          connectToDevice: function(address) {
            assert(this.deviceProxyMap.has(address), 'Device does not exist');

            return Promise.resolve({
              result: this.connectResult_,
              device: this.deviceProxyMap.get(
                  address).binding.createInterfacePtrAndBind(),
            });
          },

          getInfo: function() {
            this.methodCalled('getInfo');
            return Promise.resolve({info: this.adapterInfo_});
          },

          getDevices: function() {
            this.methodCalled('getDevices');
            return Promise.resolve({devices: this.devices_});
          },

          setClient: function(client) {
            this.methodCalled('setClient', client);
          },

          setTestAdapter: function(adapterInfo) {
            this.adapterInfo_ = adapterInfo;
          },

          setTestConnectResult: function(connectResult) {
            this.connectResult_ = connectResult;
          },

          setTestDevices: function(devices) {
            this.devices_ = devices;
            this.devices_.forEach(function(device) {
              this.deviceProxyMap.set(
                  device.address, new TestDeviceProxy(device));
            }, this);
          },
        };

        /**
         * A test Device proxy for the chrome://bluetooth-internals
         * page. Proxies are generated by a TestAdapterProxy which provides
         * the DeviceInfo.
         * @constructor
         * @extends {TestBrowserProxy}
         * @param {!device.DeviceInfo} info
         */
        var TestDeviceProxy = function(info) {
          TestBrowserProxy.call(this, [
            'getInfo',
            'getServices',
          ]);

          this.binding = new bindings.Binding(device.Device, this);
          this.info_ = info;
          this.services_ = [];
        }

        TestDeviceProxy.prototype = {
          __proto__: TestBrowserProxy.prototype,

          disconnect: function() {
            this.binding.close();
          },

          getInfo: function() {
            this.methodCalled('getInfo');
            return Promise.resolve({info: this.info_});
          },

          getServices: function() {
            this.methodCalled('getServices');
            return Promise.resolve({services: this.services_});
          },

          setTestServices: function(services) {
            this.services_ = services;
          },
        }

        frameInterfaces.addInterfaceOverrideForTesting(
            adapter.AdapterFactory.name, function(handle) {
              this.adapterFactory = new TestAdapterFactoryProxy();
              this.adapterFactory.binding.bind(handle);

              this.adapterFactory.adapter.setTestDevices([
                this.fakeDeviceInfo1(),
                this.fakeDeviceInfo2(),
              ]);
              this.adapterFactory.adapter.setTestAdapter(
                  this.fakeAdapterInfo());

              this.adapterFactory.adapter.deviceProxyMap.forEach(
                  function(deviceProxy) {
                    deviceProxy.setTestServices([
                      this.fakeServiceInfo1(),
                      this.fakeServiceInfo2(),
                    ])
                  }, this);

              this.setupResolver.resolve();
            }.bind(this));

      }.bind(this));
    }.bind(this);
  },

  /**
   * Returns a copy of fake adapter info object.
   * @return {!Object}
   */
  fakeAdapterInfo: function() {
    return {
      address: '02:1C:7E:6A:11:5A',
      discoverable: false,
      discovering: false,
      initialized: true,
      name: 'computer.example.com-0',
      powered: true,
      present: true,
    };
  },

  /**
   * Returns a copy of a fake device info object (variant 1).
   * @return {!Object}
   */
  fakeDeviceInfo1: function() {
    return {
      address: "AA:AA:84:96:92:84",
      name: "AAA",
      name_for_display: "AAA",
      rssi: {value: -40},
      services: [],
    };
  },

  /**
   * Returns a copy of a fake device info object (variant 2).
   * @return {!Object}
   */
  fakeDeviceInfo2: function() {
    return {
      address: "BB:BB:84:96:92:84",
      name: "BBB",
      name_for_display: "BBB",
      rssi: null,
      services: [],
    };
  },

  /**
   * Returns a copy of fake device info object. The returned device info lack
   * rssi and services properties.
   * @return {!Object}
   */
  fakeDeviceInfo3: function() {
    return {
      address: "CC:CC:84:96:92:84",
      name: "CCC",
      name_for_display: "CCC",
    };
  },

  /**
   * Returns a copy of fake service info object (variant 1).
   * @return {!Object}
   */
  fakeServiceInfo1: function() {
    return {
      uuid: '00002a05-0000-1000-8000-00805f9b34fb',
      is_primary: true,
    }
  },

  /**
   * Returns a copy of fake service info object (variant 2).
   * @return {!Object}
   */
  fakeServiceInfo2: function() {
    return {
      uuid: '0000180d-0000-1000-8000-00805f9b34fb',
      is_primary: true,
    }
  },

  /**
   * Returns a copy of fake characteristic info object with all properties
   * and all permissions bits set.
   * @return {!Object}
   */
  fakeCharacteristicInfo1: function() {
    return {
      id: 'characteristic1',
      uuid: '00002a19-0000-1000-8000-00805f9b34fb',
      properties: Number.MAX_SAFE_INTEGER,
      permissions: Number.MAX_SAFE_INTEGER,
      last_known_value: [],
    };
  },
};

TEST_F('BluetoothInternalsTest', 'Startup_BluetoothInternals',
    function() {
  /** @const */ var PageManager = cr.ui.pageManager.PageManager;

  var adapterFactory = null;
  var adapterFieldSet = null;
  var deviceTable = null;
  var sidebarNode = null;
  var pageNames = ['adapter', 'devices'];

  var fakeAdapterInfo = this.fakeAdapterInfo;
  var fakeDeviceInfo1 = this.fakeDeviceInfo1;
  var fakeDeviceInfo2 = this.fakeDeviceInfo2;
  var fakeDeviceInfo3 = this.fakeDeviceInfo3;
  var fakeServiceInfo1 = this.fakeServiceInfo1;
  var fakeServiceInfo2 = this.fakeServiceInfo2;
  var fakeCharacteristicInfo1 = this.fakeCharacteristicInfo1;

  // Before tests are run, make sure setup completes.
  var setupPromise = this.setupResolver.promise.then(function() {
    adapterFactory = this.adapterFactory;
  }.bind(this));

  suite('BluetoothInternalsUITest', function() {
    var EXPECTED_DEVICES = 2;

    suiteSetup(function() {
      return setupPromise.then(function() {
        return Promise.all([
          adapterFactory.whenCalled('getAdapter'),
          adapterFactory.adapter.whenCalled('getInfo'),
          adapterFactory.adapter.whenCalled('getDevices'),
          adapterFactory.adapter.whenCalled('setClient'),
        ]);
      });
    });

    setup(function() {
      adapterFieldSet = document.querySelector('#adapter fieldset');
      deviceTable = document.querySelector('#devices table');
      sidebarNode = document.querySelector('#sidebar');
      devices.splice(0, devices.length);
      adapterBroker.adapterClient_.deviceAdded(fakeDeviceInfo1());
      adapterBroker.adapterClient_.deviceAdded(fakeDeviceInfo2());
    });

    teardown(function() {
      adapterFactory.reset();
      sidebarObj.close();
      snackbar.Snackbar.dismiss(true);
      connectedDevices.clear();

      adapterFactory.adapter.deviceProxyMap.forEach(function(deviceProxy) {
        deviceProxy.reset();
      });

      PageManager.registeredPages['adapter'].setAdapterInfo(fakeAdapterInfo());

      for (var pageName in PageManager.registeredPages) {
        var page = PageManager.registeredPages[pageName];

        if (pageNames.indexOf(pageName) < 0) {
          page.pageDiv.parentNode.removeChild(page.pageDiv);
          PageManager.unregister(page);
        }
      }
    });

    /**
     * Updates device info and verifies the contents of the device table.
     * @param {!device_collection.DeviceInfo} deviceInfo
     */
    function changeDevice(deviceInfo) {
      var deviceRow = deviceTable.querySelector('#' + escapeDeviceAddress(
          deviceInfo.address));
      var nameForDisplayColumn = deviceRow.children[0];
      var addressColumn = deviceRow.children[1];
      var rssiColumn = deviceRow.children[2];
      var servicesColumn = deviceRow.children[3];

      expectTrue(!!nameForDisplayColumn);
      expectTrue(!!addressColumn);
      expectTrue(!!rssiColumn);
      expectTrue(!!servicesColumn);

      adapterBroker.adapterClient_.deviceChanged(deviceInfo);

      expectEquals(deviceInfo.name_for_display,
                   nameForDisplayColumn.textContent);
      expectEquals(deviceInfo.address, addressColumn.textContent);

      if (deviceInfo.rssi) {
        expectEquals(String(deviceInfo.rssi.value), rssiColumn.textContent);
      }

      if (deviceInfo.services) {
        expectEquals(String(deviceInfo.services.length),
                     servicesColumn.textContent);
      } else {
        expectEquals('Unknown', servicesColumn.textContent);
      }
    }

    /**
     * Escapes colons in a device address for CSS formatting.
     * @param {string} address
     */
    function escapeDeviceAddress(address) {
      return address.replace(/:/g, '\\:');
    }

    /**
     * Expects whether device with |address| is removed.
     * @param {string} address
     * @param {boolean} expectRemoved
     */
    function expectDeviceRemoved(address, expectRemoved) {
      var removedRow = deviceTable.querySelector(
          '#' + escapeDeviceAddress(address));

      expectEquals(expectRemoved, removedRow.classList.contains('removed'));
    }

    /**
     * Tests whether a device is added successfully and not duplicated.
     */
    test('DeviceAdded', function() {
      var devices = deviceTable.querySelectorAll('tbody tr');
      expectEquals(EXPECTED_DEVICES, devices.length);

      // Copy device info because device collection will not copy this object.
      var infoCopy = fakeDeviceInfo3();
      adapterBroker.adapterClient_.deviceAdded(infoCopy);

      // Same device shouldn't appear twice.
      adapterBroker.adapterClient_.deviceAdded(infoCopy);

      devices = deviceTable.querySelectorAll('tbody tr');
      expectEquals(EXPECTED_DEVICES + 1, devices.length);
    });

    /**
     * Tests whether a device is marked properly as removed.
     */
    test('DeviceSetToRemoved', function() {
      var devices = deviceTable.querySelectorAll('tbody tr');
      expectEquals(EXPECTED_DEVICES, devices.length);

      var fakeDevice = fakeDeviceInfo2();
      adapterBroker.adapterClient_.deviceRemoved(fakeDevice);

      // The number of rows shouldn't change.
      devices = deviceTable.querySelectorAll('tbody tr');
      expectEquals(EXPECTED_DEVICES, devices.length);

      expectDeviceRemoved(fakeDevice.address, true);
    });

    /**
     * Tests whether a changed device updates the device table properly.
     */
    test('DeviceChanged', function() {
      var devices = deviceTable.querySelectorAll('tbody tr');
      expectEquals(EXPECTED_DEVICES, devices.length);

      // Copy device info because device collection will not copy this object.
      var newDeviceInfo = fakeDeviceInfo1();
      newDeviceInfo.name_for_display = 'DDDD';
      newDeviceInfo.rssi = { value: -20 };
      newDeviceInfo.services = ['service1', 'service2', 'service3'];

      changeDevice(newDeviceInfo);
    });

    /**
     * Tests the entire device cycle, added -> updated -> removed -> re-added.
     */
    test('DeviceUpdateCycle', function() {
      var devices = deviceTable.querySelectorAll('tbody tr');
      expectEquals(EXPECTED_DEVICES, devices.length);

      // Copy device info because device collection will not copy this object.
      var originalDeviceInfo = fakeDeviceInfo3();
      adapterBroker.adapterClient_.deviceAdded(originalDeviceInfo);

      var newDeviceInfo = fakeDeviceInfo3();
      newDeviceInfo.name_for_display = 'DDDD';
      newDeviceInfo.rssi = { value: -20 };
      newDeviceInfo.services = ['service1', 'service2', 'service3'];

      changeDevice(newDeviceInfo);
      changeDevice(originalDeviceInfo);

      adapterBroker.adapterClient_.deviceRemoved(originalDeviceInfo);
      expectDeviceRemoved(originalDeviceInfo.address, true);

      adapterBroker.adapterClient_.deviceAdded(originalDeviceInfo);
      expectDeviceRemoved(originalDeviceInfo.address, false);
    });

    test('DeviceAddedRssiCheck', function() {
      var devices = deviceTable.querySelectorAll('tbody tr');
      expectEquals(EXPECTED_DEVICES, devices.length);

      // Copy device info because device collection will not copy this object.
      var newDeviceInfo = fakeDeviceInfo3();
      adapterBroker.adapterClient_.deviceAdded(newDeviceInfo);

      var deviceRow = deviceTable.querySelector('#' + escapeDeviceAddress(
          newDeviceInfo.address));
      var rssiColumn = deviceRow.children[2];
      expectEquals('Unknown', rssiColumn.textContent);

      var newDeviceInfo1 = fakeDeviceInfo3();
      newDeviceInfo1.rssi = {value: -42};
      adapterBroker.adapterClient_.deviceChanged(newDeviceInfo1);
      expectEquals('-42', rssiColumn.textContent);

      // Device table should keep last valid rssi value.
      var newDeviceInfo2 = fakeDeviceInfo3();
      newDeviceInfo2.rssi = null;
      adapterBroker.adapterClient_.deviceChanged(newDeviceInfo2);
      expectEquals('-42', rssiColumn.textContent);

      var newDeviceInfo3 = fakeDeviceInfo3();
      newDeviceInfo3.rssi = {value: -17};
      adapterBroker.adapterClient_.deviceChanged(newDeviceInfo3);
      expectEquals('-17', rssiColumn.textContent);
    });

    /* Sidebar Tests */
    test('Sidebar_Setup', function() {
      var sidebarItems = Array.from(
          sidebarNode.querySelectorAll('.sidebar-content li'));

      pageNames.forEach(function(pageName) {
        expectTrue(sidebarItems.some(function(item) {
          return item.dataset.pageName === pageName;
        }));
      });
    });

    test('Sidebar_DefaultState', function() {
      // Sidebar should be closed by default.
      expectFalse(sidebarNode.classList.contains('open'));
    });

    test('Sidebar_OpenClose', function() {
      sidebarObj.open();
      expectTrue(sidebarNode.classList.contains('open'));
      sidebarObj.close();
      expectFalse(sidebarNode.classList.contains('open'));
    });

    test('Sidebar_OpenTwice', function() {
      // Multiple calls to open shouldn't change the state.
      sidebarObj.open();
      sidebarObj.open();
      expectTrue(sidebarNode.classList.contains('open'));
    });

    test('Sidebar_CloseTwice', function() {
      // Multiple calls to close shouldn't change the state.
      sidebarObj.close();
      sidebarObj.close();
      expectFalse(sidebarNode.classList.contains('open'));
    });

    /* Snackbar Tests */

    /**
     * Checks snackbar showing status and returns a Promise that resolves when
     * |pendingSnackbar| is shown. If the snackbar is already showing, the
     * Promise resolves immediately.
     * @param {!snackbar.Snackbar} pendingSnackbar
     * @return {!Promise}
     */
    function whenSnackbarShows(pendingSnackbar) {
      return new Promise(function(resolve) {
        if (pendingSnackbar.classList.contains('open'))
          resolve();
        else
          pendingSnackbar.addEventListener('showed', resolve);
      });
    }

    /**
     * Performs final checks for snackbar tests.
     * @return {!Promise} Promise is fulfilled when the checks finish.
     */
    function finishSnackbarTest() {
      return new Promise(function(resolve) {
        // Let event queue finish.
        setTimeout(function() {
          expectEquals(0, $('snackbar-container').children.length);
          expectFalse(!!snackbar.Snackbar.current_);
          resolve();
        }, 50);
      });
    }

    test('Snackbar_ShowTimeout', function(done) {
      var snackbar1 = snackbar.Snackbar.show('Message 1');
      assertEquals(1, $('snackbar-container').children.length);

      snackbar1.addEventListener('dismissed', function() {
        finishSnackbarTest().then(done);
      });
    });

    test('Snackbar_ShowDismiss', function() {
      var snackbar1 = snackbar.Snackbar.show('Message 1');
      assertEquals(1, $('snackbar-container').children.length);

      return whenSnackbarShows(snackbar1).then(function() {
        return snackbar.Snackbar.dismiss();
      }).then(finishSnackbarTest);
    });

    test('Snackbar_QueueThreeDismiss', function() {
      var expectedCalls = 3;
      var actualCalls = 0;

      var snackbar1 = snackbar.Snackbar.show('Message 1');
      var snackbar2 = snackbar.Snackbar.show('Message 2');
      var snackbar3 = snackbar.Snackbar.show('Message 3');

      assertEquals(1, $('snackbar-container').children.length);
      expectEquals(2, snackbar.Snackbar.queue_.length);

      function next() {
        actualCalls++;
        return snackbar.Snackbar.dismiss();
      }

      whenSnackbarShows(snackbar1).then(next);
      whenSnackbarShows(snackbar2).then(next);
      return whenSnackbarShows(snackbar3).then(next).then(function() {
        expectEquals(expectedCalls, actualCalls);
      }).then(finishSnackbarTest);
    });

    test('Snackbar_QueueThreeDismissAll', function() {
      var expectedCalls = 1;
      var actualCalls = 0;

      var snackbar1 = snackbar.Snackbar.show('Message 1');
      var snackbar2 = snackbar.Snackbar.show('Message 2');
      var snackbar3 = snackbar.Snackbar.show('Message 3');

      assertEquals(1, $('snackbar-container').children.length);
      expectEquals(2, snackbar.Snackbar.queue_.length);

      function next() {
        assertTrue(false);
      }

      whenSnackbarShows(snackbar2).then(next);
      snackbar2.addEventListener('dismissed', next);
      whenSnackbarShows(snackbar3).then(next);
      snackbar3.addEventListener('dismissed', next);

      return whenSnackbarShows(snackbar1).then(function() {
        return snackbar.Snackbar.dismiss(true);
      }).then(function() {
        expectEquals(0, snackbar.Snackbar.queue_.length);
        expectFalse(!!snackbar.Snackbar.current_);
      }).then(finishSnackbarTest);
    });

    /* AdapterPage Tests */
    function checkAdapterFieldSet(adapterInfo) {
      for (var propName in adapterInfo) {
        var valueCell = adapterFieldSet.querySelector(
            '[data-field="' + propName + '"]');
        var value = adapterInfo[propName];

        if (typeof(value) === 'boolean') {
          expectEquals(value, valueCell.classList.contains('checked'));
        } else if (typeof(value) === 'string') {
          expectEquals(value, valueCell.textContent);
        } else {
          assert('boolean or string type expected but got ' + typeof(value));
        }
      }
    }

    test('AdapterPage_DefaultState', function() {
      checkAdapterFieldSet(adapterFieldSet.value);
    });

    test('AdapterPage_AdapterChanged', function() {
      var adapterInfo = adapterFieldSet.value;

      adapterInfo.present = !adapterInfo.present;
      adapterBroker.adapterClient_.presentChanged(adapterInfo.present);
      checkAdapterFieldSet(adapterInfo);

      adapterInfo.discovering = !adapterInfo.discovering;
      adapterBroker.adapterClient_.discoveringChanged(adapterInfo.discovering);
      checkAdapterFieldSet(adapterInfo);
    });

    test('AdapterPage_AdapterChanged_RepeatTwice', function() {
      var adapterInfo = adapterFieldSet.value;

      adapterInfo.present = !adapterInfo.present;
      adapterBroker.adapterClient_.presentChanged(adapterInfo.present);
      checkAdapterFieldSet(adapterInfo);
      adapterBroker.adapterClient_.presentChanged(adapterInfo.present);
      checkAdapterFieldSet(adapterInfo);

      adapterInfo.discovering = !adapterInfo.discovering;
      adapterBroker.adapterClient_.discoveringChanged(adapterInfo.discovering);
      checkAdapterFieldSet(adapterInfo);
      adapterBroker.adapterClient_.discoveringChanged(adapterInfo.discovering);
      checkAdapterFieldSet(adapterInfo);
    });

    /** Device Details Page Tests */

    /**
     * Checks DeviceDetailsPage status fieldset.
     * @param {!HTMLElement} detailsPage
     * @param {!Object} deviceInfo
     */
    function checkDeviceDetailsFieldSet(detailsPage, deviceInfo) {
      [
        'name',
        'address',
        'is_gatt_connected',
        'rssi.value',
        'services.length',
      ].forEach(function(propName){
        var valueCell = detailsPage.querySelector(
            'fieldset [data-field="' + propName + '"]');

        var parts = propName.split('.');
        var value = deviceInfo;

        while (value != null && parts.length > 0) {
          var part = parts.shift();
          value = value[part];
        }

        if (propName == 'is_gatt_connected')
          value = value ? 'Connected' : 'Not Connected';

        if (typeof(value) === 'boolean') {
          expectEquals(value, valueCell.classList.contains('checked'));
        } else if (typeof(value) === 'string') {
          expectEquals(value, valueCell.textContent);
        } else {
          assert('boolean or string type expected but got ' + typeof(value));
        }
      });
    }

    test('DeviceDetailsPage_NewDelete', function() {
      var device = devices.item(0);

      var deviceInspectLink = $(device.address).querySelector(
          '[is="action-link"]');

      var deviceDetailsPageId = 'devices/' + device.address.toLowerCase();

      deviceInspectLink.click();
      expectEquals("#" + deviceDetailsPageId, window.location.hash);

      var detailsPage = $(deviceDetailsPageId);
      assertTrue(!!detailsPage);

      return adapterFactory.adapter.deviceProxyMap.get(
          device.address).whenCalled('getServices').then(function() {
            // At this point, the device details page should be fully loaded.
            checkDeviceDetailsFieldSet(detailsPage, device);

            detailsPage.querySelector('.forget').click();
            expectEquals('#devices', window.location.hash);
            detailsPage = $(deviceDetailsPageId);
            expectFalse(!!detailsPage);
          });
    });

    test('DeviceDetailsPage_NewDelete_FromDevicesPage', function() {
      var device = devices.item(0);
      var deviceDetailsPageId = 'devices/' + device.address.toLowerCase();

      var deviceLinks = $(device.address).querySelectorAll(
          '[is="action-link"]');

      // First link is 'Inspect'.
      deviceLinks[0].click();
      expectEquals("#" + deviceDetailsPageId, window.location.hash);

      var detailsPage = $(deviceDetailsPageId);
      assertTrue(!!detailsPage);

      return adapterFactory.adapter.deviceProxyMap.get(
          device.address).whenCalled('getServices').then(function() {
            // At this point, the device details page should be fully loaded.
            checkDeviceDetailsFieldSet(detailsPage, device);

            // Second link is 'Forget'.
            deviceLinks[1].click();
            expectEquals('#devices', window.location.hash);
            detailsPage = $(deviceDetailsPageId);
            expectFalse(!!detailsPage);
          });
    });
  });

  suite('BluetoothInternalsUnitTests', function() {
    var ValueDataType = value_control.ValueDataType;

    /* Value Control Unit Tests */
    var aCode = 'a'.charCodeAt(0);
    var bCode = 'b'.charCodeAt(0);
    var cCode = 'c'.charCodeAt(0);

    var device1 = fakeDeviceInfo1();
    var service1 = fakeServiceInfo1();
    var characteristic1 = fakeCharacteristicInfo1();
    var valueControl = null;

    setup(function() {
      valueControl = value_control.ValueControl();
      valueControl.load(device1.address, service1.id, characteristic1);
      valueControl.typeSelect_.value = ValueDataType.HEXADECIMAL;
    });

    test('ValueControl_SetValue_Hexadecimal_EmptyArray', function() {
      valueControl.setValue([]);
      expectEquals('', valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_Hexadecimal_OneValue', function() {
      valueControl.setValue([aCode]);
      expectEquals('0x61', valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_Hexadecimal_ThreeValues', function() {
      valueControl.setValue([aCode, bCode, cCode]);
      expectEquals('0x616263', valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_UTF8_EmptyArray', function() {
      valueControl.typeSelect_.value = ValueDataType.UTF8
      valueControl.setValue([]);
      expectEquals('', valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_UTF8_OneValue', function() {
      valueControl.typeSelect_.value = ValueDataType.UTF8
      valueControl.setValue([aCode]);
      expectEquals('a', valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_UTF8_ThreeValues', function() {
      valueControl.typeSelect_.value = ValueDataType.UTF8
      valueControl.setValue([aCode, bCode, cCode]);
      expectEquals('abc', valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_Decimal_EmptyArray', function() {
      valueControl.typeSelect_.value = ValueDataType.DECIMAL
      valueControl.setValue([]);
      expectEquals('', valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_Decimal_OneValue', function() {
      valueControl.typeSelect_.value = ValueDataType.DECIMAL
      valueControl.setValue([aCode]);
      expectEquals(String(aCode), valueControl.valueInput_.value);
    });

    test('ValueControl_SetValue_Decimal_ThreeValues', function() {
      valueControl.typeSelect_.value = ValueDataType.DECIMAL
      valueControl.setValue([aCode, bCode, cCode]);
      expectEquals('97-98-99', valueControl.valueInput_.value);
    });

    test('ValueControl_ConvertValue_Hexadecimal_EmptyString', function() {
      valueControl.value_.setAs(ValueDataType.HEXADECIMAL, '');
      expectEquals(0, valueControl.value_.getArray().length);
    });

    test('ValueControl_ConvertValue_Hexadecimal_BadHexPrefix', function() {
      expectThrows(function() {
        valueControl.value_.setAs(ValueDataType.HEXADECIMAL, 'd0x');
      }, 'Expected new value to start with "0x"');
    });

    test('ValueControl_ConvertValue_Hexadecimal_ThreeValues', function() {
      valueControl.value_.setAs(ValueDataType.HEXADECIMAL, '0x616263');
      expectDeepEquals([aCode, bCode, cCode], valueControl.value_.getArray());
    });

    test('ValueControl_ConvertValue_UTF8_EmptyString', function() {
      valueControl.typeSelect_.value = ValueDataType.UTF8
      valueControl.value_.setAs(ValueDataType.UTF8, '');
      expectEquals(0, valueControl.value_.getArray().length);
    });

    test('ValueControl_ConvertValue_UTF8_ThreeValues', function() {
      valueControl.typeSelect_.value = ValueDataType.UTF8
      valueControl.value_.setAs(ValueDataType.UTF8, 'abc');
      expectDeepEquals([aCode, bCode, cCode], valueControl.value_.getArray());
    });

    test('ValueControl_ConvertValue_Decimal_EmptyString', function() {
      valueControl.typeSelect_.value = ValueDataType.DECIMAL
      valueControl.value_.setAs(ValueDataType.DECIMAL, '');
      expectEquals(0, valueControl.value_.getArray().length);
    });

    test('ValueControl_ConvertValue_Decimal_ThreeValues_Fail', function() {
      valueControl.typeSelect_.value = ValueDataType.DECIMAL

      expectThrows(function() {
        valueControl.value_.setAs(ValueDataType.DECIMAL, '97-+-99' /* a-+-c */);
      }, 'New value can only contain numbers and hyphens');
    });

    test('ValueControl_ConvertValue_Decimal_ThreeValues', function() {
      valueControl.typeSelect_.value = ValueDataType.DECIMAL
      valueControl.value_.setAs(ValueDataType.DECIMAL, '97-98-99' /* abc */);
      expectDeepEquals([aCode, bCode, cCode], valueControl.value_.getArray());
    });
  });

  // Run all registered tests.
  mocha.run();
});
