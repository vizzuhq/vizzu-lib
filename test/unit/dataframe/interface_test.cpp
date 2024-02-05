
#include "../util/test.h"
#include "dataframe/impl/dataframe.h"
#include "dataframe/interface.h"

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
		    std::make_shared<Vizzu::dataframe::dataframe>()};

		Vizzu::dataframe::dataframe_interface *operator->() const
		{
			if (!*first_check) {
				try {
					skip->*static_cast<bool>(df)
					    == "dataframe exists"_is_true;

					for (auto d : dims)
						df->add_dimension({}, {}, d, {}, {});
					for (auto m : meas)
						df->add_measure({}, m, {}, {});

					skip->*df->get_dimensions() == dims;
					skip->*df->get_measures() == meas;

					const auto ds = dims.size();
					for (auto r = 0u; r < rec.size(); ++r) {
						df->add_record(rec[r]);

						for (auto d = 0u; d < ds; ++d)
							skip->*df->get_data(r, d) == rec[r][d];

						for (auto m = 0u; m < meas.size(); ++m) {
							if (auto *d = std::get_if<double>(
							        &rec[r][m + ds]);
							    d && std::isnan(*d)) {
								auto z = df->get_data(r, m + ds);
								auto *st = std::get_if<double>(&z);
								skip->*static_cast<bool>(st)
								    == "value is a double"_is_true;
								skip->*std::isnan(*st)
								    == "value is nan"_is_true;
								continue;
							}
							skip->*df->get_data(r, m + ds)
							    == rec[r][m + ds];
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
		            }, {});
	        };

			check ->* df->get_dimensions().size() == std::size_t{};
			check ->* df->get_measures().size() == std::size_t{};
			check ->* df->get_record_count() == std::size_t{};
		}

    | "add_dimension_and_measure"_case |
		[df = setup()]
		{
			constexpr auto nan = std::numeric_limits<double>::quiet_NaN();

			df->add_dimension({{"t2", "t1", "tt3"}}, {{0, 0, 2}}, "d1", {}, {});
	        df->add_dimension({{"a"}}, {{0}}, "d0", {}, {});

	        df->add_measure({{0.0, 22.5, nan, 6.0}}, "m1", {}, {});
	        df->add_measure({{1.0}}, "m2", {}, {});

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
				}, {});

			assert ->* df->get_measures() == std::array{"m0", "m1"};

			check ->* df->get_data(std::size_t{0}, "m0") == 0.0;
			check ->* df->get_data(std::size_t{1}, "m0") == 2.0;
			check ->* df->get_data(std::size_t{2}, "m0") == -2.0;
			check ->* df->get_data(std::size_t{3}, "m0") == 6.0;

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
				}, {});

			assert ->* df->get_dimensions() == std::array{"d1", "d15", "d2"};

			check ->* df->get_data(std::size_t{0}, "d15") == "dm15dm2";
			check ->* df->get_data(std::size_t{1}, "d15") == "dm15dmX";
			check ->* df->get_data(std::size_t{2}, "d15") == "s15s2";
			check ->* df->get_data(std::size_t{3}, "d15") == "s15s3";
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
			assert ->* df->get_record_count() == std::size_t{3};

			check ->* df->get_data(std::size_t{2}, "m2") == 1.5;
			check ->* df->get_data(std::size_t{0}, "d3") == "dm3";
		}

	| "remove_records"_case |
		[df = setup({"d1"}, {"m1"}, {
				{{"dm0", NAN}},
				{{"dm1", NAN}},
				{{"dm2", NAN}},
				{{"dm3", NAN}},
				{{"dm4", NAN}},
				{{"dm5", NAN}},
				{{"dm6", NAN}},
				{{"dm7", NAN}},
				{{"dm8", 4.2}},
				{{"dm9", NAN}},
		})] {
			df->remove_records({{0ul, 2ul, 4ul, 5ul, 8ul, 9ul}});

			assert ->* df->get_measures() == std::array{"m1"};
			assert ->* df->get_dimensions() == std::array{"d1"};
			assert ->* df->get_record_count() == std::size_t{4};

			check ->* std::isnan(std::get<double>(df->get_data(std::size_t{2}, "m1")))
				== "is nan"_is_true;
			check ->* df->get_data(std::size_t{0}, "d1") == "dm6";
			check ->* df->get_data(std::size_t{1}, "d1") == "dm1";
			check ->* df->get_data(std::size_t{2}, "d1") == "dm7";
			check ->* df->get_data(std::size_t{3}, "d1") == "dm3";
		}

	| "remove_records_filter"_case |
		[df = setup({"d1"}, {"m1"}, {
				{{"dm0", 5.3}},
				{{"dm1", 2.0}},
				{{"dm2", 3.3}},
				{{"dm3", 10.1}},
				{{"dm4", 88.0}},
				{{"dm5", 2.2}},
				{{"dm6", 7.4}},
				{{"dm7", 0.0}},
				{{"dm8", 4.2}},
				{{"dm9", NAN}},
		})] {
			df->remove_records(
			[] (record_type r) -> bool
			{
				auto v = r.getValue("m1");
				return *std::get_if<double>(&v) < 5.0;
			});

			assert ->* df->get_record_count() == std::size_t{5};

			check ->* df->get_data(std::size_t{0}, "d1") == "dm0";
			check ->* df->get_data(std::size_t{1}, "d1") == "dm9";
			check ->* df->get_data(std::size_t{2}, "d1") == "dm6";
			check ->* df->get_data(std::size_t{3}, "d1") == "dm3";
			check ->* df->get_data(std::size_t{4}, "d1") == "dm4";
		}

	| "change_data"_case |
		[df = setup({"d1"}, {"m1"}, {
				{{"dm0", 5.3}},
				{{"dm1", 2.0}},
				{{"dm2", 3.3}}
		})] {
			df->change_data(std::size_t{1}, "m1", 3.0);
			df->change_data(std::size_t{2}, "d1", "dmX");

			throw_ ->* [&df]() { df->change_data(std::size_t{0}, "d1", NAN); };
			throw_ ->* [&df]() { df->change_data(std::size_t{0}, "m1", ""); };

			assert ->* df->get_record_count() == std::size_t{3};

			check ->* df->get_data(std::size_t{0}, "m1") == 5.3;
			check ->* df->get_data(std::size_t{1}, "m1") == 3.0;
			check ->* df->get_data(std::size_t{2}, "m1") == 3.3;

			check ->* df->get_data(std::size_t{0}, "d1") == "dm0";
			check ->* df->get_data(std::size_t{1}, "d1") == "dm1";
			check ->* df->get_data(std::size_t{2}, "d1") == "dmX";
		}

	| "fill_na"_case |
		[df = setup({"d1"}, {"m1"}, {
				{{"dm0", 5.3}},
				{{std::string_view{nullptr, 0}, 2.0}},
				{{"dm2", NAN}}
		})] {
			df->fill_na("m1", 3.0);
			df->fill_na("d1", "dmX");

			assert ->* df->get_record_count() == std::size_t{3};

			check ->* df->get_data(std::size_t{0}, "m1") == 5.3;
			check ->* df->get_data(std::size_t{1}, "m1") == 2.0;
			check ->* df->get_data(std::size_t{2}, "m1") == 3.0;

			check ->* df->get_data(std::size_t{0}, "d1") == "dm0";
			check ->* df->get_data(std::size_t{1}, "d1") == "dmX";
			check ->* df->get_data(std::size_t{2}, "d1") == "dm2";
		}

	| "aggregate types"_case |
		[df = setup({"d1"}, {"m1"}, {
				{{"dm0", 5.5}},
				{{"dm0", 2.0}},
				{{"dm0", 3.5}},
				{{"dm0", 10.25}},
				{{"dm0", 88.0}},
				{{"dm1", 3.5}},
				{{"dm1", 7.25}},
				{{"dm1", NAN}},
				{{"dm1", 4.25}},
				{{"dm2", NAN}},
				{{std::string_view{nullptr, 0}, 0.0}},
		})] {
			df->set_aggregate("d1", {});
			df->set_aggregate("d1", Vizzu::dataframe::aggregator_type::count);
			df->set_aggregate("d1", Vizzu::dataframe::aggregator_type::distinct);
			df->set_aggregate("d1", Vizzu::dataframe::aggregator_type::exists);
			df->set_aggregate("m1", Vizzu::dataframe::aggregator_type::sum);
			df->set_aggregate("m1", Vizzu::dataframe::aggregator_type::min);
			df->set_aggregate("m1", Vizzu::dataframe::aggregator_type::max);
			df->set_aggregate("m1", Vizzu::dataframe::aggregator_type::mean);
			df->set_aggregate("m1", Vizzu::dataframe::aggregator_type::count);
			df->set_aggregate("m1", Vizzu::dataframe::aggregator_type::distinct);
			df->set_aggregate("m1", Vizzu::dataframe::aggregator_type::exists);

			df->set_aggregate("m1", Vizzu::dataframe::custom_aggregator{
				std::string{"test"},
				[]() -> Vizzu::dataframe::custom_aggregator::id_type
				{
					return std::pair<double, double>{
						std::numeric_limits<double>::max(),
						std::numeric_limits<double>::max()
					};
				},
				[](Vizzu::dataframe::custom_aggregator::id_type &id, double v) -> double
				{
					auto &[min, min2] = std::any_cast<std::pair<double, double>&>(id);
					if (v < min) {
						min2 = min;
						min = v;
					} else if (v < min2) {
						min2 = v;
					}
					return min2;
				}
			});

			df->finalize();

			assert ->* df->get_dimensions() == std::array{
				"d1"
			};

			assert ->* df->get_measures() == std::array{
				"count(d1)", "count(m1)", "distinct(d1)", "distinct(m1)",
				"exists(d1)", "exists(m1)",
				"max(m1)", "mean(m1)", "min(m1)", "sum(m1)", "test(m1)"
			};

			assert ->* df->get_record_count() == std::size_t{4};

			check ->* df->get_data(std::size_t{0}, "count(d1)") == 5.0;
			check ->* df->get_data(std::size_t{0}, "count(m1)") == 5.0;
			check ->* df->get_data(std::size_t{0}, "distinct(d1)") == 1.0;
			check ->* df->get_data(std::size_t{0}, "distinct(m1)") == 5.0;
			check ->* df->get_data(std::size_t{0}, "exists(d1)") == 1.0;
			check ->* df->get_data(std::size_t{0}, "exists(m1)") == 1.0;
			check ->* df->get_data(std::size_t{0}, "max(m1)") == 88.0;
			check ->* df->get_data(std::size_t{0}, "mean(m1)") == 21.85;
			check ->* df->get_data(std::size_t{0}, "min(m1)") == 2.0;
			check ->* df->get_data(std::size_t{0}, "sum(m1)") == 109.25;
			check ->* df->get_data(std::size_t{0}, "test(m1)") == 3.5;

			check ->* df->get_data(std::size_t{1}, "count(d1)") == 4.0;
			check ->* df->get_data(std::size_t{1}, "count(m1)") == 3.0;
			check ->* df->get_data(std::size_t{1}, "distinct(d1)") == 1.0;
			check ->* df->get_data(std::size_t{1}, "distinct(m1)") == 3.0;
			check ->* df->get_data(std::size_t{1}, "exists(d1)") == 1.0;
			check ->* df->get_data(std::size_t{1}, "exists(m1)") == 1.0;
			check ->* df->get_data(std::size_t{1}, "max(m1)") == 7.25;
			check ->* df->get_data(std::size_t{1}, "mean(m1)") == 5.0;
			check ->* df->get_data(std::size_t{1}, "min(m1)") == 3.5;
			check ->* df->get_data(std::size_t{1}, "sum(m1)") == 15.0;
			check ->* df->get_data(std::size_t{1}, "test(m1)") == 4.25;

			check ->* df->get_data(std::size_t{2}, "count(d1)") == 1.0;
			check ->* df->get_data(std::size_t{2}, "count(m1)") == 0.0;
			check ->* df->get_data(std::size_t{2}, "distinct(d1)") == 1.0;
			check ->* df->get_data(std::size_t{2}, "distinct(m1)") == 0.0;
			check ->* df->get_data(std::size_t{2}, "exists(d1)") == 1.0;
			check ->* df->get_data(std::size_t{2}, "exists(m1)") == 0.0;
			check ->* std::isnan(std::get<double>(df->get_data(std::size_t{2}, "max(m1)"))) == "is nan"_is_true;
			check ->* std::isnan(std::get<double>(df->get_data(std::size_t{2}, "mean(m1)"))) == "is nan"_is_true;
			check ->* std::isnan(std::get<double>(df->get_data(std::size_t{2}, "min(m1)"))) == "is nan"_is_true;
			check ->* df->get_data(std::size_t{2}, "sum(m1)") == 0.0;
			check ->* df->get_data(std::size_t{2}, "test(m1)") == std::numeric_limits<double>::max();

			check ->* df->get_data(std::size_t{3}, "count(d1)") == 0.0;
			check ->* df->get_data(std::size_t{3}, "count(m1)") == 1.0;
			check ->* df->get_data(std::size_t{3}, "distinct(d1)") == 0.0;
			check ->* df->get_data(std::size_t{3}, "distinct(m1)") == 1.0;
			check ->* df->get_data(std::size_t{3}, "exists(d1)") == 0.0;
			check ->* df->get_data(std::size_t{3}, "exists(m1)") == 1.0;
		}
;
// clang-format on
