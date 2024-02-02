
#include "../util/test.h"
#include "dataframe/impl/dataframe.h"
// #include "dataframe/interface.h"

using test::assert;
using test::check;
using test::skip;
using test::throw_;
using test::operator"" _suite;
using test::operator"" _case;
using test::operator"" _is_true;
using bool_msg = test::expected_bool_t;
using Vizzu::dataframe::cell_value;
using record_type =
    Vizzu::dataframe::dataframe_interface::record_type;
using test::skip_error;

auto setup(std::initializer_list<const char *> dimensions = {},
    std::initializer_list<const char *> measures = {},
    std::initializer_list<std::vector<cell_value>> records = {})
{
	struct S
	{
		std::vector<const char *> dims;
		std::vector<const char *> meas;
		std::vector<std::vector<cell_value>> rec;
		std::shared_ptr<bool> first_check =
		    std::make_shared<bool>(false);
		std::shared_ptr<Vizzu::dataframe::dataframe_interface> df{
		    // std::make_shared<Vizzu::dataframe::dataframe>()
		};

		Vizzu::dataframe::dataframe_interface *operator->() const
		{
			if (!*first_check) {
				try {
					skip->*static_cast<bool>(df)
					    == "dataframe exists"_is_true;

					for (auto d : dims) df->add_dimension({}, {}, d);
					for (auto m : meas) df->add_measure({}, m);

					skip->*df->get_dimensions() == dims;
					skip->*df->get_measures() == meas;

					const auto ms = meas.size();
					for (auto r = 0u; r < rec.size(); ++r) {
						df->add_record(rec[r]);

						for (auto m = 0u; m < ms; ++m) {
							if (auto *d =
							        std::get_if<double>(&rec[r][m]);
							    d && std::isnan(*d)) {
								auto z = df->get_data(r, m);
								auto *st = std::get_if<double>(&z);
								skip->*static_cast<bool>(st)
								    == "value is a double"_is_true;
								skip->*std::isnan(*st)
								    == "value is nan"_is_true;
								continue;
							}
							skip->*df->get_data(r, m) == rec[r][m];
						}
						for (auto d = 0u; d < dims.size(); ++d) {
							skip->*df->get_data(r, d + ms)
							    == rec[r][d + ms];
						}
					}
				}
				catch (skip_error const &) {
					throw;
				}
				catch (std::exception const &e) {
					throw skip_error(
					    std::string{"setup failed: "} + e.what());
				}
				catch (...) {
					throw skip_error("setup failed");
				}

				*first_check = true;

				skip->*true == "setup succeeded"_is_true;
			}

			return df.get();
		}
	};

	return S{dimensions, measures, records};
};

// clang-format off
const static auto tests =
    "DataFrame::interface"_suite

    | "construct_empty"_case |
		[df = setup()]
		{
			check ->* df->get_dimensions().size() == std::size_t{};
			check ->* df->get_measures().size() == std::size_t{};
			check ->* df->get_record_count() == std::size_t{};

	        throw_ ->* [&df]() { df->add_record({}); };
	        throw_ ->* [&df]() { return df->get_data({}, {}); };
	        throw_ ->* [&df]() { return df->get_categories({}); };
	        throw_ ->* [&df]() { return df->get_min_max({}); };
			throw_ ->* [&df]()
	        {
		        return df->add_series_by_other(
		            {},
		            "",
		            [](auto, cell_value c) -> cell_value
		            {
			            return c;
		            });
	        };

			check ->* df->get_dimensions().size() == std::size_t{};
			check ->* df->get_measures().size() == std::size_t{};
			check ->* df->get_record_count() == std::size_t{};
		}

    | "add_dimension_and_measure"_case |
		[df = setup()]
		{
			constexpr auto nan = std::numeric_limits<double>::quiet_NaN();

			df->add_dimension({{"t2", "t1", "tt3"}}, {{0, 0, 2}}, "d1");
	        df->add_dimension({{"a"}}, {{0}}, "d0");

	        df->add_measure({{0.0, 22.5, nan, 6.0}}, "m1");
	        df->add_measure({{1.0}}, "m2");

	        assert ->* df->get_dimensions() == std::array{"d0", "d1"};
	        assert ->* df->get_measures() == std::array{"m1", "m2"};

	        check ->* df->get_categories("d0") == std::array{"a"};

	        assert ->* df->get_categories("d1")
	                == std::array{"t2", "t1", "tt3"};

	        check ->* df->get_min_max("m1")
	                == std::pair{0.0, 22.5};

	        check ->* df->get_min_max("m2")
	                == std::pair{1.0, 1.0};

	        df->finalize();

	        assert ->* df->get_record_count() == std::size_t{4};

	        auto check_nan = [] (cell_value const& cell, std::string&& prefix) {
		        auto str = prefix + " is a double";
		        assert ->* std::holds_alternative<double>(cell) ==
		            bool_msg{str};
		        str = prefix + " is nan";
			  	check ->* std::isnan(std::get<double>(cell)) ==
		            bool_msg{str};
	        };

	        auto check_nav = [] (cell_value const& cell, std::string&& prefix) {
		        auto str = prefix + " is a string";
		        assert ->* std::holds_alternative<std::string_view>(cell) ==
		            bool_msg{str};
		        str = prefix + " is nav";
		        check ->* (std::get<std::string_view>(cell).data() == nullptr) ==
		            bool_msg{str};
	        };

			check ->* df->get_data(std::size_t{0}, "m1") == 0.0;
			check ->* df->get_data(std::size_t{1}, "m1") == 22.5;
	        check_nan(df->get_data(std::size_t{2}, "m1"), "table_20");
	        check ->* df->get_data(std::size_t{3}, "m1") == 6.0;

	        check ->* df->get_data(std::size_t{0}, "m2") == 1.0;
	        check_nan(df->get_data(std::size_t{1}, "m2"), "table_11");
	        check_nan(df->get_data(std::size_t{2}, "m2"), "table_21");
	        check_nan(df->get_data(std::size_t{3}, "m2"), "table_31");

	        check ->* df->get_data(std::size_t{0}, "d0") == "a";
	        check_nav(df->get_data(std::size_t{1}, "d0"), "table_12");
	        check_nav(df->get_data(std::size_t{2}, "d0"), "table_22");
	        check_nav(df->get_data(std::size_t{3}, "d0"), "table_32");

	        assert->* df->get_data(std::size_t{0}, "d1") == "t2";
	        check ->* df->get_data(std::size_t{1}, "d1") == "t2";
	        check ->* df->get_data(std::size_t{2}, "d1") == "tt3";
	        check_nav(df->get_data(std::size_t{3}, "d1"), "table_33");

	        check ->* (std::get<std::string_view>(df->get_data(std::size_t{0}, "d1")).data() ==
	            			df->get_categories("d1")[0].data()) ==
	            "Not points to the same memory address"_is_true;
		}

    | "add_record"_case |
		[df = setup({"test_dim"}, {"test_meas"})]
		{
			df->add_record({{"test_dim_val", 2.0}});
			df->add_record({{-1.0, "test_dim_val2"}});

	        throw_ ->* [&df]() { df->add_record({}); };
	        throw_ ->* [&df]() { df->add_record({{0.0}}); };
	        throw_ ->* [&df]() { df->add_record({{0.0, 0.0}}); };
	        throw_ ->* [&df]() { df->add_record({{"test", "t"}}); };
	        throw_ ->* [&df]()
	        {
		        df->add_record({{0.0, "test", 0.0}});
	        };
	        throw_ ->* [&df]()
	        {
		        df->add_record({{0.0, "test", "test"}});
	        };

			df->finalize();

	        assert ->* df->get_record_count() == std::size_t{2};

	        check ->* df->get_categories("test_dim")
	                == std::array{"test_dim_val", "test_dim_val2"};

	        check ->* df->get_min_max("test_meas") == std::pair{-1.0, 2.0};

	        check ->* df->get_data({}, "test_meas") == 2.0;
	        check ->* df->get_data({}, "test_dim")
	            == "test_dim_val";

	        check ->* df->get_data(std::size_t{1}, "test_meas") == -1.0;
	        check ->* df->get_data(std::size_t{1}, "test_dim")
	            == "test_dim_val2";
		}

	| "add_series_by_other"_case |
		[df = setup({"d1", "d2"}, {"m1"}, {
				{{"dm1", "dm2", 0.0}},
				{{"dm1", "dmX", 1.0}},
				{{"s1", "s2", -1.0}},
				{{"s1", "s3", 3.0}}
			})] {

			df->add_series_by_other(
				"m1",
				"m0",
				[](auto, cell_value c) -> cell_value
				{
					const double* v = std::get_if<double>(&c);
					assert ->* static_cast<bool>(v)
					    == "value is a double"_is_true;

					return *v * 2;
				});

			assert ->* df->get_measures() == std::array{"m0", "m1"};

			check ->* df->get_data(std::size_t{0}, {}) == 0.0;
			check ->* df->get_data(std::size_t{1}, {}) == 2.0;
			check ->* df->get_data(std::size_t{2}, {}) == -2.0;
			check ->* df->get_data(std::size_t{3}, {}) == 6.0;

			df->add_series_by_other(
				"d1",
				"d15",
				[](const record_type& r, cell_value c) -> cell_value
				{
					auto v = std::get_if<std::string_view>(&c);
					skip ->* static_cast<bool>(v)
					    == "value is string"_is_true;

					auto oth_v = r.getValue("d2");

					auto v2 = std::get_if<std::string_view>(&oth_v);
					skip ->* static_cast<bool>(v2)
					    == "value is string"_is_true;

					thread_local std::string val;
					val = std::string{*v} + "5" + std::string{*v2};

					return val;
				});

			assert ->* df->get_dimensions() == std::array{"d1", "d15", "d2"};

			check ->* df->get_data(std::size_t{0}, std::size_t{3}) == "dm15dm2";
			check ->* df->get_data(std::size_t{1}, std::size_t{3}) == "dm15dmX";
			check ->* df->get_data(std::size_t{2}, std::size_t{3}) == "s15s2";
			check ->* df->get_data(std::size_t{3}, std::size_t{3}) == "s15s3";
		}

	| "remove_series"_case |
		[df = setup({"d1", "d2", "d3"}, {"m1", "m2", "m3"}, {
				{{"dm1", "dx2", "dm3", 0.0, 0.1, 0.2}},
				{{"dm1", "dx2", "am3", 1.0, 2.1, 3.2}},
				{{"dm2", "dm2", "bm3", 1.0, 1.5, 1.2}},
		})] {
			df->remove_series({{"m1", "d2", "m3"}});

			assert ->* df->get_measures() == std::array{"m2"};
			assert ->* df->get_dimensions() == std::array{"d1", "d3"};

			check ->* df->get_data(std::size_t{2}, "m2") == 1.5;
			check ->* df->get_data(std::size_t{0}, "d3") == "dm3";

		}

;
// clang-format on
